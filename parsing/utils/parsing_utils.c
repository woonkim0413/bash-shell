/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:18:01 by rakim             #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/05/11 19:25:28 by woonkim          ###   ########.fr       */
=======
/*   Updated: 2025/05/12 14:31:07 by rakim            ###   ########.fr       */
>>>>>>> 186cdea (heardoc)
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

<<<<<<< HEAD
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

static	void	print_cmd(t_object *object, t_cmd_info *cmd_info)
=======
static	void	print_cmd(t_cmd_info *cmd_info)
>>>>>>> 186cdea (heardoc)
{
	int			idx;
	t_redirect	*redirect;

	redirect = cmd_info->redirect;
	print_log(1, object, "=========================\n");
	print_log(1, object, "cmd : %s\n", cmd_info->cmd);
	idx = 0;
	while (cmd_info->evecve_argv[idx])
	{
		print_log(1, object, "evecve_argv : %s\n", cmd_info->evecve_argv[idx]);
		idx++;
	}
	print_log(1, object, "cmd_path : %s\n", cmd_info->cmd_path);
	while (redirect)
	{
		print_log(1, object, "redirect_type : %u\n", redirect->type);
		print_log(1, object, "redirect_file_path : %s\n", redirect->file_path);
		redirect = redirect->next;
	}
	print_log(1, object, "=========================\n");
}

void	print_all_cmd(t_object *object)
{
	t_cmd_info	*temp;
	
	temp = object->cmd_info;
	while (temp)
	{
		print_cmd(object, temp);
		temp = temp->next;
	}
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
	{
		free((*line));
		throw_error("argv error", object, NULL);
	}
	while ((*line)[idx])
	{
		if (before_pipe && (*line)[idx] == '|')
		{
			free((*line));
			throw_error("argv error", object, NULL);
		}
		if ((*line)[idx] == '|')
			before_pipe = 1;
		else
			before_pipe = 0;
		idx++;
	}
}
