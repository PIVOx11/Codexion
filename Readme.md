*This project has been created as part of the 42 curriculum by blidriss.*

# Codexion

## Description

Codexion is a multithreaded resource scheduling simulator written in C using POSIX threads. The project models a set of coders competing for shared dongles in order to compile their code.

Each coder repeatedly performs the following cycle:

1. Request the required dongles.
2. Compile.
3. Debug.
4. Refactor.

A coder requires two dongles to compile. Since each dongle is shared between neighboring coders, concurrent access must be carefully synchronized to prevent race conditions, deadlocks, starvation, and inconsistent resource states.

The simulator supports two scheduling policies:

* **FIFO (First In, First Out)**: the first coder requesting a dongle receives priority.
* **EDF (Earliest Deadline First)**: the coder closest to burnout receives priority.

The project focuses on thread synchronization, resource scheduling, condition variables, mutexes, and concurrent system design.

---

## Features

* Multithreaded simulation using POSIX threads.
* FIFO scheduler.
* EDF scheduler.
* Per-dongle waiting queues.
* Cooldown period after dongle release.
* Dedicated monitor thread.
* Burnout detection.
* Thread-safe logging.
* Graceful cleanup on initialization failures.

---

## Instructions

### Compilation

```bash
make
```

### Execution

```bash
./codexion 5 900 100 100 100 3 100 fifo
```

Parameters:

| Argument          | Description                                             |
| ----------------- | ------------------------------------------------------- |
| number_of_coders  | Number of coder threads                                 |
| burnout_time      | Maximum time a coder can remain without compiling       |
| compile_time      | Duration of compilation                                 |
| debug_time        | Duration of debugging                                   |
| refactor_time     | Duration of refactoring                                 |
| required_compiles | Number of compilations required before a coder finishes |
| cooldown_time     | Mandatory cooldown after a dongle release               |
| scheduler         | fifo or edf                                         |

---
#### ⚠️ Notice that all parameter are mondatory

## Scheduler Design

Each coder is associated with two dongles

Each dongle maintains a small waiting queue containing the coders currently requesting it.

Before compiling, a coder:

1. Adds a request to both required dongles.
2. Waits until it becomes the winner for each dongle.
3. Acquires both dongles.
4. Compiles.
5. Releases both dongles and wakes waiting coders.

---

## Blocking Cases Handled

### Deadlock Prevention

All coders acquire dongles in a globally consistent order.

This prevents circular wait situations and breaks one of Coffman's conditions required for deadlock.

### Starvation Prevention

The EDF scheduler prioritizes coders with the earliest deadline, allowing coders closest to burnout to obtain resources before less urgent requests.

### Resource Contention

Each dongle maintains its own waiting queue and scheduling policy, ensuring deterministic ownership decisions when multiple coders compete for the same resource.

### Cooldown Handling

After a dongle is released, a configurable cooldown period is enforced before the dongle can be reused.

This simulates a temporary resource recovery phase and prevents immediate reacquisition.

### Burnout Detection

A dedicated monitor thread continuously checks all coders.

A coder is considered burned out when the time elapsed since its last compilation exceeds the configured burnout threshold.

### Log Serialization

All console output is protected by a dedicated mutex.

This guarantees that messages are printed atomically and prevents interleaved or corrupted logs.

---

## Thread Synchronization Mechanisms

### pthread_mutex_t

Mutexes are used to protect all shared state.

Examples include:

* Dongle ownership state.
* Dongle waiting queues.
* Monitor state.
* Simulation termination state.
* Console output.

This prevents concurrent modifications and race conditions.

### pthread_cond_t

Each dongle owns a condition variable.

Coders waiting for a dongle block using:

```c
pthread_cond_wait(...)
```

When a dongle becomes available, waiting coders are notified using:

```c
pthread_cond_broadcast(...)
```

This avoids busy waiting and allows efficient thread synchronization.

### Thread-Safe Communication

Communication between coders and the monitor occurs through protected shared variables.

Access to shared state is always synchronized through mutex-protected getters and setters, ensuring visibility and consistency between threads.

### Race Condition Prevention

Race conditions are prevented by:

* Protecting shared variables with mutexes.
* Using condition variables for synchronization.
* Updating resource ownership atomically.
* Serializing access to shared scheduler structures.

---

## Resources
[operating_system concepts](https://www.tutorialspoint.com/operating_system/os_process_scheduling.htm)

[playlist for posix Thread in c](https://youtu.be/d9s_d28yJq0?si=jggQNPmGR9biVBXy)

### AI Usage

AI tools were used during the development process for:

* Concurrency theory discussions.
* Deadlock and starvation analysis.
* Thread synchronization explanations.
* Test case generation.
