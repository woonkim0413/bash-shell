/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:18:01 by rakim             #+#    #+#             */
/*   Updated: 2025/05/19 14:56:31 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static	int	is_double_pipe(int *before_pipe, char **line, \
	int idx, t_object *object)
{
	if (*before_pipe && (*line)[idx] == '|')
	{
		throw_error("argv error", object, NULL, line);
		return (0);
	}
	if ((*line)[idx] == '|')
		*before_pipe = 1;
	else
		*before_pipe = 0;
	return (1);
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
			if (is_double_pipe(&before_pipe, line, idx, object))
				return ;
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
