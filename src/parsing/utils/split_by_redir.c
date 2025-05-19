/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_by_redir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:59:54 by rakim             #+#    #+#             */
/*   Updated: 2025/05/15 14:20:30 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	void	set_in_word(char *s, int *idx, int *in_word, int *count)
{
	if ((s[*idx] == '>' && s[*idx + 1] == '>') || \
	(s[*idx] == '<' && s[*idx + 1] == '<') || \
	(s[*idx] == '>' && s[*idx + 1] != '>') || \
	(s[*idx] == '<' && s[*idx + 1] != '<'))
	{
		if ((s[*idx == '>'] && s[*idx + 1] == '>') || \
		(s[*idx == '<'] && s[*idx + 1] == '<'))
			(*idx)++;
		*in_word = 0;
		(*count)++;
	}
}

static	int	check_redir(char *s)
{
	int	idx;
	int	in_single;
	int	in_double;
	int	count;
	int	in_word;

	idx = 0;
	in_single = 0;
	in_double = 0;
	count = 0;
	in_word = 0;
	while (s[idx])
	{
		set_toggle(s[idx], &in_single, &in_double);
		if (!in_word)
		{
			in_word = 1;
			count++;
		}
		if (!in_single && !in_double)
			set_in_word(s, &idx, &in_word, &count);
		idx++;
	}
	return (count);
}

static	int	count_redir(char **temp)
{
	int	count;
	int	idx;

	count = 0;
	idx = 0;
	while (temp[idx])
	{
		count += check_redir(temp[idx]);
		idx++;
	}
	return (count);
}

char	**split_redir_with_quote(char **temp)
{
	t_result_info	result_info;
	int				total_count;

	total_count = count_redir(temp);
	result_info.result = ft_calloc(total_count + 1, sizeof(char *));
	result_info.result_idx = 0;
	split_by_redir(temp, &result_info);
	return (result_info.result);
}
