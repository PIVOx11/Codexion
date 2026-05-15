CC = cc
CFLAGS =  -Wall -Wextra -Werror -pthread
NAME = codexion
HEADERE = coders/codexion.h
SRC = coders/codexion.c coders/parsing_stage.c coders/time.c
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