/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:18:01 by rakim             #+#    #+#             */
/*   Updated: 2025/05/18 20:33:26 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	void	print_cmd(t_object *object, t_cmd_info *cmd_info)
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
		if (redirect->type == TOKEN_APPEND)
			print_log(1, object, "redirect_type : %s\n", ">>");
		if (redirect->type == TOKEN_HEREDOC)
			print_log(1, object, "redirect_type : %s\n", "<<");
		if (redirect->type == TOKEN_REDIR_IN)
			print_log(1, object, "redirect_type : %s\n", "<");
		if (redirect->type == TOKEN_REDIR_OUT)
			print_log(1, object, "redirect_type : %s\n", ">");
		print_log(1, object, "redirect_file_path : %s\n", redirect->file_path);
		redirect = redirect->next;
	}
	print_log(1, object, "=========================\n");
}

void	print_all_cmd(t_object *object)
{
	t_cmd_info	*temp;

	temp = object->cmd_info;
	if (!temp)
		return ;
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
	int	in_single;
	int	in_double;

	idx = 0;
	before_pipe = 0;
	in_single = 0;
	in_double = 0;
	if ((*line)[idx] == '|')
	{
		object->last_exit_status = 2;
		throw_error("argv error", object, NULL, line);
		return ;
	}
	while ((*line)[idx])
	{
		set_toggle((*line)[idx], &in_single, &in_double);
		if (!in_single && !in_double)
		{
			if (before_pipe && (*line)[idx] == '|')
			{
				throw_error("argv error", object, NULL, line);
				return ;
			}
			if ((*line)[idx] == '|')
				before_pipe = 1;
			else
				before_pipe = 0;
		}
		idx++;
	}
}

void	set_toggle(char c, int *in_single, int *in_double)
{
	if (c == SINGLE_QUOTE_ASCII && !(*in_double))
		*in_single = !(*in_single);
	if (c == DOUBLE_QUOTE_ASCII && !(*in_single))
		*in_double = !(*in_double);
}
