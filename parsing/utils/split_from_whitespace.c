/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_from_whitespace.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 10:18:15 by rakim             #+#    #+#             */
/*   Updated: 2025/04/21 18:44:53 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	int	ft_isspace(char c)
{
	if (c == ' ' || (c >= 9 && c <= 13))
		return (1);
	return (0);
}

static	int	count_words(const char *s)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] && ft_isspace((unsigned char)s[i]))
			i++;
		if (s[i])
		{
			count++;
			while (s[i] && !ft_isspace((unsigned char)s[i]))
				i++;
		}
	}
	return (count);
}

static	char	*word_dup(const char *start, int len)
{
	char	*word;
	int		idx;

	idx = 0;
	word = (char *)malloc(len + 1);
	if (!word)
		return (NULL);
	while (idx < len)
	{
		word[idx] = start[idx];
		idx++;
	}
	word[len] = '\0';
	return (word);
}

static	void	set_word(char **line, int idx, char **result)
{
	int		len;
	char	*start;

	start = (*line);
	len = 0;
	while (*line && !ft_isspace(**line))
	{
		len++;
		(*line)++;
	}
	result[idx] = word_dup(start, len);
}

char	**ft_split_from_whitespace(char *line)
{
	int		words;
	char	**result;
	int		idx;

	words = count_words(line);
	result = (char **)malloc((words + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	idx = 0;
	while (*line)
	{
		while (*line && ft_isspace((unsigned char)*line))
			line++;
		if (*line)
		{
			set_word(&line, idx, result);
			idx++;
		}
	}
	result[idx] = NULL;
	return (result);
}
