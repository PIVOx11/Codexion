CC = cc

CFLAGS = -Wall -Wextra -Werror -pthread -g

NAME = codexion

HEADER = coders/codexion.h

SRC = codexion.c \
parsing_stage.c \
init_stage.c \
cleanup_stage.c \
simulation_stage.c \
action_stage.c \
monitor.c \
set_and_get.c \
safety.c \
scheduler.c \
scheduler_utils.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all fclean clean re