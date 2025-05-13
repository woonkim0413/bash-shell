/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_with_quote.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 20:07:38 by rakim             #+#    #+#             */
/*   Updated: 2025/05/12 14:34:30 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	int	count_word_split_by_space(char *s)
{
	int	in_word;
	int	count;
	int	idx;
	int	in_single;
	int	in_double;

	count = 0;
	idx = 0;
	in_single = 0;
	in_double = 0;
	in_word = 0;
	while (s[idx])
	{
		if (!in_single && !in_double && s[idx] == ' ')
			in_word = 0;
		if (!in_word && !in_single && !in_double && s[idx] != ' ')
		{
			in_word = 1;
			count++;
		}
		set_toggle(s[idx], &in_single, &in_double);
		idx++;
	}
	return (count);
}

static	void	split_by_space(char ***result, char *s)
{
	int	start;
	int	idx;
	int	in_single;
	int	in_double;
	int	word_count;

	idx = 0;
	in_single = 0;
	in_double = 0;
	start = -1;
	word_count = 0;
	while (s[idx])
	{
		if (start == -1 && s[idx] != ' ')
			start = idx;
		if (!in_single && !in_double && s[idx] == ' ' && start != -1)
		{
			(*result)[word_count++] = ft_substr(s, start, idx - start);
			start = -1;
		}
		set_toggle(s[idx], &in_single, &in_double);
		idx++;
	}
	if (start != -1)
		(*result)[word_count] = ft_substr(s, start, idx);
}

char	**split_by_space_with_quote(char *s)
{
	char	**result;
	int		word_count;
	char	**temp;

	if (!s)
		return (NULL);
	word_count = count_word_split_by_space(s);
	temp = ft_calloc(word_count + 1, sizeof(char *));
	split_by_space(&temp, s);
	result = split_redir_with_quote(temp);
	return (result);
}
