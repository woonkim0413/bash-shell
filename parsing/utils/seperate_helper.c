/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   seperate_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 19:32:26 by rakim             #+#    #+#             */
/*   Updated: 2025/05/10 15:31:10 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_toggle(char c, int *in_single, int *in_double)
{
	if (c == SINGLE_QUOTE_ASCII && !(*in_double))
		*in_single = !(*in_single);
	if (c == DOUBLE_QUOTE_ASCII && !(*in_single))
		*in_double = !(*in_double);
}

static	void	process_seperate_line(char ***result, char **line, \
	t_object *object, int result_idx)
{
	int		in_single;
	int		in_double;
	int		end;
	int		start;

	in_single = 0;
	in_double = 0;
	end = 0;
	start = 0;
	while ((*line)[end])
	{
		set_toggle((*line)[end], &in_single, &in_double);
		if ((*line)[end] == '|' && !in_double && !in_single)
		{
			(*result)[result_idx] = ft_substr(*line, start, end - start);
			result_idx++;
			start = end + 1;
			end++;
		}
		if ((*line)[end] == '$' && !in_single)
			extend_env(line, &end, object);
		end++;
	}
	(*result)[result_idx] = ft_substr(*line, start, ft_strlen(*line) - start);
	free(*line);
}

static	int	count_pipe_not_in_quote(char *line)
{
	int		count;
	int		idx;
	int		in_single;
	int		in_double;
	char	c;

	count = 1;
	idx = 0;
	in_single = 0;
	in_double = 0;
	while (line[idx])
	{
		c = line[idx];
		if (c == '\'' && !in_double)
			in_single = !in_single;
		else if (c == '"' && !in_single)
			in_double = !in_double;
		else if (c == '|' && !in_double && !in_single)
			count++;
		idx++;
	}
	return (count);
}

char	**extend_env_and_split(char **line, t_object *object)
{
	char	**result;

	result = ft_calloc(sizeof(char *), count_pipe_not_in_quote(*line) + 1);
	if (!result)
	{
		free(*line);
		throw_error("malloc_error", object, NULL);
	}
	process_seperate_line(&result, line, object, 0);
	return (result);
}
