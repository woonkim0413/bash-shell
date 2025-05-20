NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g -Isrc/include
LDFLAGS = -lreadline

SRCS = src/minishell.c \
src/print_log.c \
src/error/error_handler.c \
src/init/init_signal.c \
src/init/init_helper.c \
src/init/init_utils.c \
src/free/free.c \
src/free/free_helper.c \
src/parsing/parsing_token.c \
src/parsing/utils/seperate_helper.c \
src/parsing/utils/parsing_utils.c \
src/parsing/utils/split_by_space_with_quote.c \
src/parsing/utils/split_by_redir.c \
src/parsing/utils/split_by_redir_2.c \
src/parsing/heredoc/handle_heredoc.c \
src/parsing/heredoc/handle_heredoc_utils.c \
src/parsing/env/extend_env.c \
src/parsing/env/env_helper.c \
src/parsing/quote/quote_handler.c \
src/parsing/quote/clean_up_quote.c \
src/parsing/redirect/redirect_parsing_handler.c \
src/imple_cmd/redirect_setting.c \
src/imple_cmd/imp_utils1.c \
src/imple_cmd/imp_utils2.c \
src/imple_cmd/parent_wait_to_child.c \
src/imple_cmd/cmd_path_find.c \
src/imple_cmd/implement.c \
src/imple_cmd/imple_setting.c \
src/imple_cmd/builtins/echo.c \
src/imple_cmd/builtins/env.c \
src/imple_cmd/builtins/exit.c \
src/imple_cmd/builtins/export.c \
src/imple_cmd/builtins/export_utils.c \
src/imple_cmd/builtins/pwd.c \
src/imple_cmd/builtins/unset.c \
src/imple_cmd/builtins/cd.c \
src/imple_cmd/builtins/cd_utils.c \
src/imple_cmd/builtins/builtin_utils.c \
src/imple_cmd/help_except_case.c \

OBJS = $(SRCS:.c=.o)
HEADER = src/include/minishell.h

LIBFT_DIR = src/libft
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