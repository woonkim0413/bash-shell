/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_with_quote.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 20:07:38 by rakim             #+#    #+#             */
/*   Updated: 2025/04/26 20:28:07 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_word(char const *s)
{
	int	count;
	int	in_word;
	int	in_single;
	int	in_double;
	int	idx;

	count = 0;
	in_word = 0;
	in_single = 0;
	in_double = 0;
	idx = 0;
	while (s[idx])
	{
		set_toggle(s[idx], &in_single, &in_double);
		if (!in_single && !in_double && (s[idx] == ' '))
			in_word = 0;
		else if (!in_word)
		{
			in_word = 1;
			count++;
		}
		idx++;
	}
	return (count);
}

static char	*alloc_word(char const *s, int start, int end)
{
	char	*word;
	int		i;

	word = ft_calloc(end - start + 1, sizeof(char));
	if (!word)
		return (NULL);
	i = 0;
	while (start < end)
		word[i++] = s[start++];
	return (word);
}

static	void	process_set_result(char ***result, const char *s, int idx)
{
	int		start;
	int		word_idx;
	int		in_single;
	int		in_double;

	start = -1;
	word_idx = 0;
	in_single = 0;
	in_double = 0;
	while (s[idx])
	{
		set_toggle(s[idx], &in_single, &in_double);
		if (!in_single && !in_double && s[idx] == ' ')
		{
			if (start != -1)
				(*result)[word_idx++] = alloc_word(s, start, idx);
			start = -1;
		}
		else if (start == -1)
			start = idx;
		idx++;
	}
	if (start != -1)
		(*result)[word_idx++] = alloc_word(s, start, idx);
}

char	**split_with_quote(char const *s)
{
	char	**result;

	if (!s)
		return (NULL);
	result = ft_calloc(count_word(s) + 1, sizeof(char *));
	if (!result)
		return (NULL);
	process_set_result(&result, s, 0);
	return (result);
}
