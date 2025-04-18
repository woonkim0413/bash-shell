NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g

SRCS = minishell.c
OBJS = $(SRCS:.c=.o)
HEADER = minishell.h

LIBFT_NAME = libft.a
LIBFT_DIR = libft
LIBFT_PATH = $(LIBFT_DIR)/$(LIBFT_NAME)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_PATH)
	$(CC) $(OBJS) $(LIBFT_PATH) -o $@

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT_PATH):
		@$(MAKE) -C $(LIBFT_DIR)

clean:
		rm -rf $(OBJS)
		$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
		rm -rf $(NAME)
		$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re