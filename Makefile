NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g -Iinclude
LDFLAGS = -lreadline

SRCS = minishell.c \
error/error_handler.c \
init/init_signal.c \
init/init_helper.c \
init/init_utils.c \
parsing/parsing_token.c \
parsing/seperate_helper.c \
parsing/utils/parsing_utils.c \
parsing/utils/split_with_quote.c \
parsing/env/extend_env.c \
parsing/env/env_helper.c \
parsing/quote/quote_handler.c \
parsing/quote/clean_up_quote.c \
imple_cmd/imp_utils1.c \
imple_cmd/imp_utils2.c \
imple_cmd/imp_utils3.c \
imple_cmd/cmd_path_find.c \
imple_cmd/implement.c \
imple_cmd/builtins/echo.c \
imple_cmd/builtins/env.c \
imple_cmd/builtins/exit.c \
imple_cmd/builtins/export.c \
imple_cmd/builtins/pwd.c \
imple_cmd/builtins/unset.c \
imple_cmd/builtins/cd.c \

OBJS = $(SRCS:.c=.o)
HEADER = include/minishell.h

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