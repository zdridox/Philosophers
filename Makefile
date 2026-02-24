NAME = philo

CC = cc
CFLAGS = -Wall -Werror -Wextra -g

SRC = src/main.c src/philo.c src/state_utils.c \
 src/utils.c src/validation.c src/philo_actions.c src/control_thread.c

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

.PHONY: all clean fclean re