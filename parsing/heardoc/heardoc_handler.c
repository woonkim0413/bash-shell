/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heardoc_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 15:31:13 by rakim             #+#    #+#             */
/*   Updated: 2025/05/17 19:18:55 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	volatile sig_atomic_t sigint_flag = 0;

static	void	find_dollar_location(char **line, t_object *object)
{
	int		idx;

	idx = 0;
	while ((*line)[idx])
	{
		if ((*line)[idx] == '$' && \
		(ft_isalnum((*line)[idx + 1]) || \
		(*line)[idx + 1] == '_'))
			extend_env(line, &idx, object);
		idx++;
	}
}

static	void	sigint_handler_in_child(int signal)
{
	(void)signal;
	sigint_flag = 1;
	rl_done = 1;
}

static	void	child_process(t_object *object, t_redirect *redirect, \
	int pipe_fd[2])
{
	char	*line;

	signal(SIGINT, sigint_handler_in_child);
	signal(SIGQUIT, SIG_DFL);
	close(pipe_fd[0]);
	while (1)
	{
		line = readline("> ");
		if (sigint_flag || !line)
		{
			free(line);
			break ;
		}
		if (!ft_strncmp(line, redirect->file_path, ft_strlen(line)))
		{
			free(line);
			break ;
		}
		if (is_have_quotes(redirect->file_path))
			check_quotes(&line, object);
		else
			find_dollar_location(&line, object);
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	close(pipe_fd[1]);
	free_object(object);
	if (sigint_flag)
		exit(130);
	else
		exit(0);
}

static	void	write_heardoc_in_pipe(t_object *object, t_redirect *redirect)
{
	int		pipe_fd[2];
	int		pid;
	int		status;

	if (pipe(pipe_fd) == -1)
		throw_error("pipe_error", object, NULL, NULL);
	pid = fork();
	if (pid == 0)
		child_process(object, redirect, pipe_fd);
	else
	{
		waitpid(pid, &status, 0);
		close(pipe_fd[1]);
		redirect->heredoc_fd = pipe_fd[0];
		object->last_exit_status = WEXITSTATUS(status);
		if (object->last_exit_status == 130)
		{
			close(pipe_fd[0]);
			object->heredoc_interrupted = 1;
			free_cmd_info(&(object->cmd_info));
			return ;
		}
	}
}

void	handle_heardoc(t_object *object)
{
	t_cmd_info	*cmd_info;
	t_redirect	*redirect;

	if (!object->cmd_info)
		return ;
	cmd_info = object->cmd_info;
	while (cmd_info)
	{
		redirect = cmd_info->redirect;
		while (redirect)
		{
			if (redirect->type == TOKEN_HEREDOC)
				write_heardoc_in_pipe(object, redirect);
			if (!object->cmd_info)
				return ;
			redirect = redirect->next;
		}
		cmd_info = cmd_info->next;
	}
}
