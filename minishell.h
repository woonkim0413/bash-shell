/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 16:54:25 by rakim             #+#    #+#             */
/*   Updated: 2025/04/20 16:05:35 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "signal.h"

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_cmd_info
{
	char				*cmd;
	char				**evecve_argv;
	char				*cmd_path;
	int					input_fd;
	int					output_fd;
	int					pipline_flag;
	struct s_cmd_info	*prev;
	struct s_cmd_info	*next;
}	t_cmd_info;

t_env	*init(int length, char *input[], char *env[]);
void	init_signal(void);
void	init_child_signal(void);
void	throw_error(char *message, t_env *head);
int		is_all_space(const char *line);
void	print_env_list(t_env *env_list);

#endif
