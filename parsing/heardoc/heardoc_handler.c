/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heardoc_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 15:31:13 by rakim             #+#    #+#             */
/*   Updated: 2025/05/12 17:23:28 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static	void	write_heardoc_in_pipe(t_object *object, t_redirect *redirect)
{
	int		pipe_fd[2];
	char	*line;

	if (pipe(pipe_fd) == -1)
		throw_error("pipe_error", object, NULL);
	while (1)
	{
		line = readline("> ");
		if (!line || !ft_strncmp(line, redirect->file_path, ft_strlen(line)))
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
	redirect->heredoc_fd = pipe_fd[0];
}

void	handle_heardoc(t_object *object)
{
	t_cmd_info	*cmd_info;
	t_redirect	*redirect;

	cmd_info = object->cmd_info;
	while (cmd_info)
	{
		redirect = cmd_info->redirect;
		while (redirect)
		{
			if (redirect->type == TOKEN_HEREDOC)
				write_heardoc_in_pipe(object, redirect);
			redirect = redirect->next;
		}
		cmd_info = cmd_info->next;
	}
}
