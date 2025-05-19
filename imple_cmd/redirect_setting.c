/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_setting.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 17:02:03 by woonkim           #+#    #+#             */
/*   Updated: 2025/05/19 10:37:07 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	redirect_process3(t_imp_stus *imp_stus, t_redirect *redirect);
static void	redirect_process2(t_imp_stus *imp_stus, t_redirect *redirect);
static void	redirect_process(t_object *object, t_imp_stus *imp_stus, \
	int one_builtin_flag);

void	input_output_setting(t_object *object, t_imp_stus *imp_stus, \
		int one_builtin_flag)
{
	if (!one_builtin_flag)
	{
		close(imp_stus->stderr_pipe[0]);
		dup2(imp_stus->stderr_pipe[1], STDERR_FILENO);
		close(imp_stus->stderr_pipe[1]);
		close(imp_stus->pipeFd[imp_stus->cur_c_n][0]);
		if (object->cmd_info->prev != NULL)
		{
			dup2(imp_stus->pipeFd[imp_stus->cur_c_n - 1][0], STDIN_FILENO);
			close(imp_stus->pipeFd[imp_stus->cur_c_n - 1][0]);
		}
		if (object->cmd_info->next != NULL)
			dup2(imp_stus->pipeFd[imp_stus->cur_c_n][1], STDOUT_FILENO);
		close(imp_stus->pipeFd[imp_stus->cur_c_n][1]);
	}
	redirect_process(object, imp_stus, one_builtin_flag);
}

static void	redirect_process(t_object *object, t_imp_stus *imp_stus, \
	int one_builtin_flag)
{
	t_redirect	*redirect;

	redirect = object->cmd_info->redirect;
	while (redirect)
	{
		if ((redirect->type != TOKEN_HEREDOC && \
			redirect->type != TOKEN_REDIR_OUT) && \
			access(redirect->file_path, F_OK) != 0)
		{
			write(2, redirect->file_path, ft_strlen(redirect->file_path));
			write(2, ": No such file or directitory\n", 31);
			if (one_builtin_flag)
			{
				imp_stus->all_path = 1;
				return ;
			}
			free_stus_and_object(object, imp_stus);
			exit(1);
		}
		redirect_process2(imp_stus, redirect);
		redirect_process3(imp_stus, redirect);
		redirect = redirect->next;
	}
}

static void	redirect_process2(t_imp_stus *imp_stus, t_redirect *redirect)
{
	if (redirect->type == TOKEN_REDIR_IN)
	{
		imp_stus->input_fd = open(redirect->file_path, O_RDONLY);
		dup2(imp_stus->input_fd, STDIN_FILENO);
		close(imp_stus->input_fd);
	}
	else if (redirect->type == TOKEN_REDIR_OUT)
	{
		imp_stus->output_fd = open(redirect->file_path, \
			O_WRONLY | O_CREAT | O_TRUNC, 0644);
		dup2(imp_stus->output_fd, STDOUT_FILENO);
		close(imp_stus->output_fd);
	}
}

static void	redirect_process3(t_imp_stus *imp_stus, t_redirect *redirect)
{
	if (redirect->type == TOKEN_APPEND)
	{
		imp_stus->output_fd = open(redirect->file_path, \
							O_WRONLY | O_CREAT | O_APPEND, 0644);
		dup2(imp_stus->output_fd, STDOUT_FILENO);
		close(imp_stus->output_fd);
	}
	else if (redirect->type == TOKEN_HEREDOC)
	{
		imp_stus->input_fd = redirect->heredoc_fd;
		dup2(imp_stus->input_fd, STDIN_FILENO);
		close(imp_stus->input_fd);
	}	
}
