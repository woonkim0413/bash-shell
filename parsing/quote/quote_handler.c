/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:50:00 by rakim             #+#    #+#             */
/*   Updated: 2025/04/27 14:41:20 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	int	is_have_quotes(char *line)
{
	int	idx;

	idx = 0;
	while (line[idx])
	{
		if (line[idx] == DOUBLE_QUOTE_ASCII)
			return (1);
		if (line[idx] == SINGLE_QUOTE_ASCII)
			return (2);
		idx++;
	}
	return (0);
}

static	void	is_quote_close(char **line, t_object *object)
{
	int		in_duoble;
	int		in_single;
	int		idx;
	char	c;

	in_single = 0;
	in_duoble = 0;
	idx = 0;
	while ((*line)[idx])
	{
		c = (*line)[idx];
		if (c == '\'' && !in_duoble)
			in_single = !in_single;
		else if (c == '"' && !in_single)
			in_duoble = !in_duoble;
		else if (c == '\\' && in_duoble && (*line)[idx + 1] == '"')
			idx++;
		idx++;
	}
	if (in_single || in_duoble)
	{
		free((*line));
		throw_error("quote is not close", object);
	}
}

void	check_quotes(char **line, t_object *object)
{
	if (is_have_quotes(*line))
		is_quote_close(line, object);
}

// cat "$HOME/Desktop/minishell/minishell.c" | wc -al | echo '"$HOME" is my home' | echo "'$HOME'"