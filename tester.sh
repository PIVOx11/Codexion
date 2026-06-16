#!/bin/bash
# =============================================================================
#  Codexion Tester
#  Usage: ./tester.sh [path/to/codexion]
#  Default: looks for ./codexion in current directory
# =============================================================================

BIN="${1:-./codexion}"
PASS=0
FAIL=0
WARN=0
TOLERANCE=15   # ms tolerance for timing checks
HANG_TIMEOUT=5 # seconds before we call it a hang

# ── colours ──────────────────────────────────────────────────────────────────
RED='\033[0;31m'; GREEN='\033[0;32m'; YELLOW='\033[1;33m'
BLUE='\033[0;34m'; CYAN='\033[0;36m'; BOLD='\033[1m'; RESET='\033[0m'

# ── helpers ───────────────────────────────────────────────────────────────────
section() { echo -e "\n${BOLD}${BLUE}══════════════════════════════════════════${RESET}"; echo -e "${BOLD}${BLUE}  $1${RESET}"; echo -e "${BOLD}${BLUE}══════════════════════════════════════════${RESET}"; }
ok()      { echo -e "  ${GREEN}✔${RESET}  $1"; ((PASS++)); }
fail()    { echo -e "  ${RED}✘${RESET}  ${RED}$1${RESET}"; ((FAIL++)); }
warn()    { echo -e "  ${YELLOW}⚠${RESET}  ${YELLOW}$1${RESET}"; ((WARN++)); }
info()    { echo -e "  ${CYAN}·${RESET}  $1"; }

run() {
    # run BIN with args, timeout, capture stdout+stderr separately
    local args="$1"
    local timeout_s="${2:-$HANG_TIMEOUT}"
    OUTPUT=$(timeout "$timeout_s" bash -c "$BIN $args" 2>/tmp/codexion_stderr)
    EXIT_CODE=$?
    STDERR_OUT=$(cat /tmp/codexion_stderr)
    if [ $EXIT_CODE -eq 124 ]; then HUNG=1; else HUNG=0; fi
}

# ── pre-flight ─────────────────────────────────────────────────────────────
echo -e "${BOLD}Codexion Tester${RESET}"
echo -e "Binary: ${CYAN}$BIN${RESET}"

if [ ! -f "$BIN" ]; then
    echo -e "${RED}ERROR: Binary '$BIN' not found.${RESET}"
    echo "Usage: $0 [path/to/codexion]"
    exit 1
fi
if [ ! -x "$BIN" ]; then
    echo -e "${RED}ERROR: '$BIN' is not executable.${RESET}"
    exit 1
fi
echo -e "Timing tolerance: ±${TOLERANCE}ms\n"

# =============================================================================
# 1. ARGUMENT VALIDATION
# =============================================================================
section "1 · Argument Validation"

invalid_cases=(
    ""
    "1 800 200 200 200 5 0"
    "1 800 200 200 200 5 0 fifo extra"
    "-1 800 200 200 200 5 0 fifo"
    "0 800 200 200 200 5 0 fifo"
    "abc 800 200 200 200 5 0 fifo"
    "3.5 800 200 200 200 5 0 fifo"
    "3 800 200 200 200 5 0 bad_sched"
    "9999999999 800 200 200 200 5 0 fifo"
    "3 -1 200 200 200 5 0 fifo"
    "3 800 -1 200 200 5 0 fifo"
)
descs=(
    "no arguments"
    "7 args (missing scheduler)"
    "9 args (extra argument)"
    "negative coder count (-1)"
    "zero coders (0)"
    "letters in numeric field (abc)"
    "float in numeric field (3.5)"
    "invalid scheduler name"
    "integer overflow in first arg"
    "negative burnout (-1)"
    "negative compile time (-1)"
)

for i in "${!invalid_cases[@]}"; do
    run "${invalid_cases[$i]}" 3
    if [ $HUNG -eq 1 ]; then
        fail "HANG on invalid: ${descs[$i]}"
    elif [ $EXIT_CODE -ne 0 ]; then
        ok "exit 1 on invalid: ${descs[$i]}"
    else
        fail "exit 0 (expected exit 1) on invalid: ${descs[$i]}"
    fi
done

# leading + should be accepted
run "+3 +800 +200 +100 +100 +2 +0 fifo" 5
if [ $HUNG -eq 1 ]; then
    fail "HANG on leading + args"
elif [ $EXIT_CODE -eq 0 ]; then
    ok "leading + prefix accepted"
else
    warn "leading + prefix: exited $EXIT_CODE (some implementations reject this)"
fi

# =============================================================================
# 2. SINGLE CODER
# =============================================================================
section "2 · Single Coder (num_coders=1)"

run "1 800 200 200 200 5 0 fifo" 3
if [ $HUNG -eq 1 ]; then
    fail "HANG: single coder never exits"
else
    ok "single coder exits (no hang)"
    if [ $EXIT_CODE -eq 0 ]; then ok "exit code 0"; else fail "exit code $EXIT_CODE (expected 0)"; fi

    burnout_count=$(echo "$OUTPUT" | grep -c "burned out")
    if [ "$burnout_count" -eq 1 ]; then ok "exactly 1 burned out line"; else fail "burned out appeared $burnout_count times (expected 1)"; fi

    dongle_count=$(echo "$OUTPUT" | grep -c "has taken a dongle")
    if [ "$dongle_count" -eq 0 ]; then ok "no 'has taken a dongle' (single coder cannot compile)";
    else fail "'has taken a dongle' appeared $dongle_count times (single coder should not compile)"; fi

    compile_count=$(echo "$OUTPUT" | grep -c "is compiling")
    if [ "$compile_count" -eq 0 ]; then ok "no 'is compiling' for single coder";
    else fail "'is compiling' appeared $compile_count times (should be 0)"; fi

    burnout_ts=$(echo "$OUTPUT" | grep "burned out" | awk '{print $1}')
    if [ -n "$burnout_ts" ]; then
        diff=$((burnout_ts - 800))
        if [ $diff -lt 0 ]; then diff=$((-diff)); fi
        if [ $diff -le $TOLERANCE ]; then ok "burnout timing: ${burnout_ts}ms (expected ~800ms, diff=${diff}ms)";
        else fail "burnout timing: ${burnout_ts}ms (expected ~800ms, diff=${diff}ms > ${TOLERANCE}ms tolerance)"; fi
    fi
fi

# =============================================================================
# 3. BURNOUT DETECTION
# =============================================================================
section "3 · Burnout Detection"

# 3a: zero burnout fires immediately
run "3 0 200 100 100 5 0 fifo" 3
if [ $HUNG -eq 1 ]; then
    fail "HANG: zero burnout never exits"
else
    ok "zero burnout: program exits"
    burnout_count=$(echo "$OUTPUT" | grep -c "burned out")
    if [ "$burnout_count" -ge 1 ]; then ok "zero burnout: burned out line printed";
    else fail "zero burnout: no burned out line"; fi
fi

# 3b: only ONE burnout line ever (run 10 times)
info "Running burnout uniqueness check (10 runs)..."
burnout_multi_fail=0
for i in $(seq 1 10); do
    run "4 350 200 100 100 10 0 fifo" 4
    count=$(echo "$OUTPUT" | grep -c "burned out")
    if [ "$count" -gt 1 ]; then burnout_multi_fail=1; fi
done
if [ $burnout_multi_fail -eq 0 ]; then ok "never more than 1 burnout line (10 runs)";
else fail "multiple 'burned out' lines appeared in same run"; fi

# 3c: program must exit after burnout (no hang)
run "3 400 200 200 200 10 0 fifo" 4
if [ $HUNG -eq 1 ]; then fail "HANG after burnout fires";
else ok "program exits after burnout"; fi

# 3d: burnout timing accuracy
run "2 500 200 200 200 10 0 fifo" 4
if [ $HUNG -eq 0 ]; then
    burnout_ts=$(echo "$OUTPUT" | grep "burned out" | head -1 | awk '{print $1}')
    if [ -n "$burnout_ts" ]; then
        diff=$((burnout_ts - 500))
        if [ $diff -lt 0 ]; then diff=$((-diff)); fi
        if [ $diff -le $TOLERANCE ]; then ok "burnout timing accurate: ${burnout_ts}ms (expected ~500ms, diff=${diff}ms)";
        else fail "burnout timing off: ${burnout_ts}ms (expected ~500ms, diff=${diff}ms > ${TOLERANCE}ms)"; fi
    fi
fi

# =============================================================================
# 4. LOG FORMAT
# =============================================================================
section "4 · Log Format"

run "3 800 200 100 100 2 0 fifo" 5
if [ $HUNG -eq 0 ] && [ -n "$OUTPUT" ]; then
    # Every line must match: NUMBER NUMBER WORD...
    bad_lines=$(echo "$OUTPUT" | grep -vE '^[0-9]+ [0-9]+ .+$')
    if [ -z "$bad_lines" ]; then ok "all lines match 'TIMESTAMP ID MESSAGE' format";
    else fail "malformed lines found:"; echo "$bad_lines" | head -5 | while read l; do echo "      > $l"; done; fi

    # Coder IDs must be in range 1..N (here N=3)
    bad_ids=$(echo "$OUTPUT" | awk '$2 < 1 || $2 > 3 {print}')
    if [ -z "$bad_ids" ]; then ok "all coder IDs in range [1..3]";
    else fail "coder IDs out of range: $bad_ids"; fi

    # Timestamps must be non-negative
    neg_ts=$(echo "$OUTPUT" | awk '$1 < 0 {print}')
    if [ -z "$neg_ts" ]; then ok "all timestamps >= 0";
    else fail "negative timestamps found"; fi

    # Timestamps must be monotonically non-decreasing
    non_mono=$(echo "$OUTPUT" | awk 'NR>1{if($1<prev) print NR": "$0} {prev=$1}')
    if [ -z "$non_mono" ]; then ok "timestamps are non-decreasing";
    else warn "timestamps decreased (may be a race in printing): line $non_mono"; fi

    # Valid action words only
    bad_actions=$(echo "$OUTPUT" | awk '{
        action = ""
        for(i=3;i<=NF;i++) action=action" "$i
        if (action != " has taken a dongle" && action != " is compiling" && \
            action != " is debugging" && action != " is refactoring" && \
            action != " burned out") print $0
    }')
    if [ -z "$bad_actions" ]; then ok "all action messages are valid";
    else fail "unknown action messages:"; echo "$bad_actions" | head -3 | while read l; do echo "      > $l"; done; fi
fi

# =============================================================================
# 5. COMPILE SEQUENCE INVARIANTS
# =============================================================================
section "5 · Per-Coder Sequence Invariants"

run "2 1200 200 100 100 3 0 fifo" 8
if [ $HUNG -eq 0 ]; then
    # Each coder must get exactly 2 dongle lines per compile
    total_compiles=$(echo "$OUTPUT" | grep -c "is compiling")
    total_dongles=$(echo "$OUTPUT" | grep -c "has taken a dongle")
    expected_dongles=$((total_compiles * 2))
    if [ "$total_dongles" -eq "$expected_dongles" ]; then
        ok "dongle lines = compiles × 2 ($total_dongles = $total_compiles × 2)"
    else
        fail "dongle count mismatch: $total_dongles dongles for $total_compiles compiles (expected $expected_dongles)"
    fi

    # Per-coder: validate state machine order for each coder
    # dongle -> dongle -> compile -> debug -> refactor (repeat)
    info "Checking per-coder state machine..."
    coder_fail=0
    for cid in 1 2; do
        seq=$(echo "$OUTPUT" | awk -v id="$cid" '$2==id {
            if ($3=="has") print "D"
            else if ($3=="is" && $4=="compiling")  print "C"
            else if ($3=="is" && $4=="debugging")   print "G"
            else if ($3=="is" && $4=="refactoring") print "R"
            else if ($3=="burned") print "B"
        }' | tr -d '\n')
        # Valid patterns: (DDC(GR)*)+ or (DDC(GR)*)B
        if echo "$seq" | grep -qE '^(DDC(GR)*)+$' || echo "$seq" | grep -qE '^(DDC(GR)*)*(DDC)?B?$'; then
            ok "coder $cid state machine valid: $seq"
        else
            fail "coder $cid bad sequence: $seq"
            coder_fail=1
        fi
    done
fi

# =============================================================================
# 6. SUCCESSFUL COMPLETION (no burnout)
# =============================================================================
section "6 · Successful Completion (all compile req times)"

# 2 coders, generous time
run "2 1500 200 100 100 3 0 fifo" 8
if [ $HUNG -eq 1 ]; then
    fail "HANG: 2 coders 3 compiles"
else
    ok "2 coders 3 compiles: exits without hang"
    if [ $EXIT_CODE -eq 0 ]; then ok "exit code 0"; else fail "exit code $EXIT_CODE"; fi
    burnout=$(echo "$OUTPUT" | grep -c "burned out")
    if [ "$burnout" -eq 0 ]; then ok "no burnout (completed cleanly)";
    else warn "burnout occurred ($burnout) — may indicate timing issue with these params"; fi
    c1=$(echo "$OUTPUT" | awk '$2==1 && $3=="is" && $4=="compiling"' | wc -l)
    c2=$(echo "$OUTPUT" | awk '$2==2 && $3=="is" && $4=="compiling"' | wc -l)
    if [ "$c1" -ge 3 ] && [ "$c2" -ge 3 ]; then ok "both coders compiled ≥3 times (c1=$c1 c2=$c2)";
    else warn "compile counts lower than expected: c1=$c1 c2=$c2 (may be burnout-limited)"; fi
fi

# 3 coders
run "3 1500 200 100 100 2 0 fifo" 8
if [ $HUNG -eq 1 ]; then fail "HANG: 3 coders 2 compiles";
else ok "3 coders 2 compiles: exits"; fi

# =============================================================================
# 7. COOLDOWN
# =============================================================================
section "7 · Dongle Cooldown"

COOLDOWN=300
run "3 3000 200 100 100 2 $COOLDOWN fifo" 10
if [ $HUNG -eq 1 ]; then
    fail "HANG with cooldown=$COOLDOWN"
else
    ok "exits with cooldown=$COOLDOWN"

    info "Checking dongle re-acquisition gaps >= ${COOLDOWN}ms..."
    # For each dongle (shared between adjacent coders), collect acquisition timestamps.
    # We can't easily track which physical dongle was used from logs alone,
    # so we verify: for each coder, the gap between their OWN dongle acquisitions
    # must be at least: compile_time + cooldown (they must wait for cooldown after releasing)
    coder_gap_fail=0
    for cid in 1 2 3; do
        prev_ts=""
        while IFS= read -r ts; do
            if [ -n "$prev_ts" ]; then
                gap=$((ts - prev_ts))
                # gap must be >= compile_time (200) + cooldown (300) = 500 minimum
                # actually the second dongle acquisition of same coder: gap should be >= cooldown
                # The minimum cycle is: compile(200) + debug(100) + refactor(100) + cooldown(300) = 700
                if [ $gap -lt $COOLDOWN ]; then
                    fail "coder $cid: dongle re-acquired too fast (gap=${gap}ms < cooldown=${COOLDOWN}ms)"
                    coder_gap_fail=1
                fi
            fi
            prev_ts=$ts
        done < <(echo "$OUTPUT" | awk -v id="$cid" '$2==id && $3=="has" && $4=="taken" {print $1}' | awk 'NR%2==1')
    done
    if [ $coder_gap_fail -eq 0 ]; then ok "cooldown gaps respected for all coders"; fi
fi

# zero cooldown should not add delay
run "2 800 200 100 100 2 0 fifo" 5
if [ $HUNG -eq 0 ]; then ok "zero cooldown: program exits normally"; fi

# =============================================================================
# 8. FIFO SCHEDULER
# =============================================================================
section "8 · FIFO Scheduler"

fifo_hang=0
for n in 2 3 4 5 7; do
    run "$n 1200 200 100 100 2 0 fifo" 8
    if [ $HUNG -eq 1 ]; then
        fail "HANG: $n coders fifo"
        fifo_hang=1
    else
        ok "$n coders FIFO: exits (exit=$EXIT_CODE)"
    fi
done

run "5 900 200 100 100 3 0 fifo" 8
if [ $HUNG -eq 0 ]; then
    burnout=$(echo "$OUTPUT" | grep -c "burned out")
    if [ "$burnout" -le 1 ]; then ok "FIFO 5 coders: at most 1 burnout";
    else fail "FIFO 5 coders: $burnout burnouts (must be ≤1)"; fi
fi

# =============================================================================
# 9. EDF SCHEDULER
# =============================================================================
section "9 · EDF Scheduler"

for n in 2 3 5; do
    run "$n 1200 200 100 100 2 0 edf" 8
    if [ $HUNG -eq 1 ]; then fail "HANG: $n coders edf";
    else ok "$n coders EDF: exits (exit=$EXIT_CODE)"; fi
done

# EDF uniqueness: only 1 burnout
info "Running EDF burnout uniqueness check (5 runs)..."
edf_multi=0
for i in $(seq 1 5); do
    run "4 400 200 100 100 10 0 edf" 4
    count=$(echo "$OUTPUT" | grep -c "burned out")
    if [ "$count" -gt 1 ]; then edf_multi=1; fi
done
if [ $edf_multi -eq 0 ]; then ok "EDF: never more than 1 burnout (5 runs)";
else fail "EDF: multiple burnouts in same run"; fi

# =============================================================================
# 10. EDGE CASES
# =============================================================================
section "10 · Edge Cases"

# zero compiles required: runs until burnout
run "3 400 200 100 100 0 0 fifo" 4
if [ $HUNG -eq 1 ]; then
    fail "HANG: 0 compiles required"
else
    ok "0 compiles required: exits"
    burnout=$(echo "$OUTPUT" | grep -c "burned out")
    if [ "$burnout" -eq 1 ]; then ok "0 compiles: ends with burnout (as expected)";
    else fail "0 compiles: expected burnout, got $burnout burnout lines"; fi
fi

# burnout == compile time: the waiting coder starves
run "2 200 200 100 100 5 0 fifo" 4
if [ $HUNG -eq 1 ]; then fail "HANG: burnout == compile_time";
else ok "burnout == compile_time: exits"; fi

# burnout < compile time
run "2 100 200 100 100 5 0 fifo" 4
if [ $HUNG -eq 1 ]; then fail "HANG: burnout < compile_time";
else ok "burnout < compile_time: exits"; fi

# large number of coders
run "100 2000 200 100 100 1 0 fifo" 10
if [ $HUNG -eq 1 ]; then fail "HANG: 100 coders";
else ok "100 coders: exits (exit=$EXIT_CODE)"; fi

# max coders
run "250 2000 200 100 100 1 0 fifo" 12
if [ $HUNG -eq 1 ]; then fail "HANG: 250 coders (CODER_MAX)";
else ok "250 coders: exits (exit=$EXIT_CODE)"; fi

# =============================================================================
# 11. REPEATED RUNS (stability)
# =============================================================================
section "11 · Stability (20 repeated runs)"

info "Running 20x: 5 coders 800ms burnout 2 compiles fifo..."
hang_count=0
nonzero_count=0
for i in $(seq 1 20); do
    run "5 800 200 100 100 2 0 fifo" 5
    if [ $HUNG -eq 1 ]; then ((hang_count++)); fi
    # exit code 0 is always expected (either done or burnout)
done
if [ $hang_count -eq 0 ]; then ok "0 hangs in 20 runs";
else fail "$hang_count hangs in 20 runs"; fi

info "Running 20x: 5 coders edf..."
hang_count_edf=0
for i in $(seq 1 20); do
    run "5 800 200 100 100 2 0 edf" 5
    if [ $HUNG -eq 1 ]; then ((hang_count_edf++)); fi
done
if [ $hang_count_edf -eq 0 ]; then ok "0 hangs in 20 EDF runs";
else fail "$hang_count_edf hangs in 20 EDF runs"; fi

# =============================================================================
# SUMMARY
# =============================================================================
echo ""
echo -e "${BOLD}══════════════════════════════════════════${RESET}"
echo -e "${BOLD}  RESULTS${RESET}"
echo -e "${BOLD}══════════════════════════════════════════${RESET}"
TOTAL=$((PASS + FAIL + WARN))
echo -e "  Total tests : $TOTAL"
echo -e "  ${GREEN}Passed${RESET}      : $PASS"
echo -e "  ${RED}Failed${RESET}      : $FAIL"
echo -e "  ${YELLOW}Warnings${RESET}    : $WARN"
echo ""
if [ $FAIL -eq 0 ]; then
    echo -e "  ${GREEN}${BOLD}ALL TESTS PASSED${RESET}"
else
    echo -e "  ${RED}${BOLD}$FAIL TEST(S) FAILED${RESET}"
fi
echo ""
exit $FAIL
