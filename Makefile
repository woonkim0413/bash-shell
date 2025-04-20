NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g
LDFLAGS = -lreadline

SRCS = minishell.c error_handler.c signal_handler.c init_helper.c untils.c
OBJS = $(SRCS:.c=.o)
HEADER = minishell.h

LIBFT_DIR = libft
LIBFT_NAME = libft.a
LIBFT_PATH = $(LIBFT_DIR)/$(LIBFT_NAME)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_PATH)
	$(CC) $(OBJS) $(LIBFT_PATH) $(LDFLAGS) -o $@

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