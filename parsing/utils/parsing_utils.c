/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:18:01 by rakim             #+#    #+#             */
/*   Updated: 2025/04/26 17:34:37 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_string_arr(char **string_arr)
{
	int	idx;

	idx = 0;
	while (string_arr[idx])
	{
		free(string_arr[idx]);
		idx++;
	}
	free(string_arr);
}

void	print_cmd(t_cmd_info *cmd_info)
{
	int			idx;
	t_redirect	*redirect;

	redirect = cmd_info->redirect;
	printf("=========================\n");
	printf("cmd : %s\n", cmd_info->cmd);
	idx = 0;
	while (cmd_info->evecve_argv[idx])
	{
		printf("evecve_argv : %s\n", cmd_info->evecve_argv[idx]);
		idx++;
	}
	printf("cmd_path : %s\n", cmd_info->cmd_path);
	printf("input_fd : %d\n", cmd_info->input_fd);
	printf("output_fd : %d\n", cmd_info->output_fd);
	while (redirect)
	{
		printf("redirect_type : %u\n", redirect->type);
		printf("redirect_file_path : %s\n", redirect->file_path);
		redirect = redirect->next;
	}
	printf("=========================\n");
}

void	whitespace_convert_to_space(char **line)
{
	int	idx;

	idx = 0;
	while ((*line)[idx])
	{
		if ((*line)[idx] >= 9 && (*line)[idx] <= 13)
			(*line)[idx] = ' ';
		idx++;
	}
}

void	check_pipe(char **line, t_object *object)
{
	int	idx;
	int	before_pipe;

	idx = 0;
	before_pipe = 0;
	if ((*line)[idx] == '|')
		throw_error("argv error", object);
	while ((*line)[idx])
	{
		if (before_pipe && (*line)[idx] == '|')
		{
			free((*line));
			throw_error("argv error", object);
		}
		if ((*line)[idx] == '|')
			before_pipe = 1;
		else
			before_pipe = 0;
		idx++;
	}
}
