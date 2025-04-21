/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 16:54:25 by rakim             #+#    #+#             */
/*   Updated: 2025/04/21 19:25:53 by woonkim          ###   ########.fr       */
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
	struct s_cmd_info	*prev;
	struct s_cmd_info	*next;
}	t_cmd_info;

typedef struct s_object
{
	t_cmd_info	*cmd_info;
	t_env		*env;
}	t_object;

/* init */
void		init(int length, char *input[], t_object *object, char **env);
void		init_signal(void);
void		init_child_signal(void);
/* init utils*/
int			is_all_space(const char *line);
/* error */
void		throw_error(char *message, t_object *object);
/* parsing */
void		parsing(char **line_splited_pipe, t_object *object);
/* parsing/utils */
char		**ft_split_from_whitespace(char *line);
void		free_string_arr(char **string_arr);
void		handle_env(char **src, int idx, t_env *env);
void		print_cmd(t_cmd_info *cmd_info);

/* ./imple_cmd/cmd_path_find.c*/
void	find_path(t_cmd_info* t_cmd, t_env* env);

#endif

