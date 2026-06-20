CC = cc

CFLAGS = -Wall -Wextra -Werror -pthread

NAME = codexion

SRC = src/codexion.c \
src/parsing_stage.c \
src/init_stage.c \
src/cleanup_stage.c \
src/simulation_stage.c \
src/action_stage.c \
src/monitor.c \
src/set_and_get.c \
src/safety.c \
src/scheduler.c \
src/scheduler_utils.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all fclean clean re