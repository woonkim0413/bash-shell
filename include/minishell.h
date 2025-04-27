/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 16:54:25 by rakim             #+#    #+#             */
/*   Updated: 2025/04/27 14:46:02 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>

# define DOLLAR_ASCII 36
# define DOUBLE_QUOTE_ASCII 34
# define SINGLE_QUOTE_ASCII 39

typedef enum e_token_type
{
	TOKEN_REDIR_IN, // <
	TOKEN_REDIR_OUT, // >
	TOKEN_HEREDOC, // <<
	TOKEN_APPEND // >>
}	t_token_type;

typedef struct	s_env
{
	char			*key;
	char			*value;
	struct	s_env	*next;
}	t_env;

typedef struct s_redirect
{
	t_token_type		type;
	// file_path free 해야함
	char				*file_path;
	struct s_redirect	*next;
}	t_redirect;

typedef struct s_cmd_info
{
	char				*cmd;
	char				**evecve_argv;
	char				*cmd_path;
	int					input_fd;
	int					output_fd;
	t_redirect			*redirect;
	int					heredoc_fd;
	struct s_cmd_info	*prev;
	struct s_cmd_info	*next;
}	t_cmd_info;

typedef struct s_object
{
	t_cmd_info	*cmd_info;
	t_env		*env;
	int			last_exit_status;
}	t_object;

/* error */
void		throw_error(char *message, t_object *object);
void		free_all(t_object *object);
/* init */
void		init(int length, char *input[], t_object *object, char **env);
void		init_signal(void);
void		init_child_signal(void);
/* init utils*/
int			is_all_space(const char *line);
/* parsing */
void		parsing(char **line_splited_pipe, t_object *object);
/* parsing/quote/quote_handler */
void		check_quotes(char **line, t_object *object);
/* parsing/quote/clean_up_quote */
void		clean_up_quote(t_cmd_info *cmd_info);
/* parsing/seperate_helper */
char		**extend_env_and_split(char **line, t_object *object);
void		set_toggle(char c, int *in_single, int *in_double);
/* parsing/env/env_helper */
char		*get_env(char *key, t_env *env);
/* parsing/env/extend_env */
char		*extract_key_in(char *src);
void		extend_env(char **line, int *dolloar_idx, t_object *object);
/* parsing/utils */
void		check_pipe(char **line, t_object *object);
void		free_string_arr(char **string_arr);
void		print_all_cmd(t_cmd_info *cmd_info);
void		whitespace_convert_to_space(char **line);
char		**split_with_quote(char const *s);

/* ./imple_cmd/cmd_path_find.c*/
void		find_path(t_cmd_info *t_cmd, t_env *env);

#endif

