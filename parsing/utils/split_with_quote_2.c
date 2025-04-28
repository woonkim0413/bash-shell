/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_with_quote_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 14:39:33 by rakim             #+#    #+#             */
/*   Updated: 2025/04/28 20:33:07 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
char	*alloc_word(char const *s, int start, int end)
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

int	next_delim_len(const char *s, int idx)
{
	if (s[idx] == '<' && s[idx + 1] == '<')
		return (2);
	if (s[idx] == '>' && s[idx + 1] == '>')
		return (2);
	if (s[idx] == '<' || s[idx] == '>')
		return (1);
	return (0);
}

void	set_result(t_for_split_with_quote *arg, int *start)
{
	int		dlen;

	dlen = next_delim_len(arg->s, arg->idx);
	if (arg->s[arg->idx] == ' ')
	{
		if (*start != -1)
			(arg->result)[arg->word_idx++] = \
			alloc_word(arg->s, *start, arg->idx);
		*start = -1;
		if (arg->s[arg->idx + 1])
			*start = arg->idx + 1;
	}
	else if (dlen)
	{
		if (*start != -1)
			(arg->result)[arg->word_idx++] = \
			alloc_word(arg->s, *start, arg->idx);
		(arg->result)[arg->word_idx++] = \
		alloc_word(arg->s, arg->idx, arg->idx + dlen);
		arg->idx += dlen;
		*start = -1;
	}
}

int	check_delim(char *s, int idx, int *count, int *in_word)
{
	int	dlen;

	dlen = next_delim_len(s, idx);
	if (s[idx] == ' ')
	{
		(*in_word) = 0;
		dlen = 1;
	}
	else if (dlen)
	{
		(*in_word) = 0;
	}
	(void)count;
	return (dlen);
}
*/

static	void	set_in_word(char *s, int idx, int *in_word, int *count)
{
	if ((s[idx] == '>' && s[idx + 1] == '>') || \
	(s[idx] == '<' && s[idx + 1] == '<') || \
	(s[idx] == '>' && s[idx + 1] != '>') || \
	(s[idx] == '<' && s[idx + 1] != '<'))
	{
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
			set_in_word(s, idx, &in_word, &count);
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

static	void	process_split_by_redir(char *s, char **result, int *result_idx)
{
	int	s_idx;
	int	in_single;
	int	in_double;
	int	start;

	s_idx = 0;
	in_single = 0;
	in_double = 0;
	start = -1;
	while (s[s_idx])
	{
		if (start == -1)
			start = s_idx;
		set_toggle(s[s_idx], &in_single, &in_double);
		if (!in_single && !in_double)
		{
			if ((s[s_idx] == '>' && s[s_idx + 1] == '>') || (s[s_idx] == '<' && s[s_idx + 1] == '<'))
			{
				result[(*result_idx)++] = ft_substr(s, start, s_idx - start);
				result[(*result_idx)++] = ft_substr(s, s_idx, 2);
				start = -1;
				s_idx++;
			}
			else if (s[s_idx] == '>' || s[s_idx] == '<')
			{
				result[(*result_idx)++] = ft_substr(s, start, s_idx - start);
				result[(*result_idx)++] = ft_substr(s, s_idx, 1);
				start = -1;
			}
		}
		s_idx++;
	}
	if (start != -1)
		result[(*result_idx)] = ft_substr(s, s_idx, s_idx - start);
}

static	void	split_by_redir(char **temp, char **result)
{
	int	temp_idx;
	int	result_idx;

	result_idx = 0;
	temp_idx = 0;
	while (temp[temp_idx])
	{
		process_split_by_redir(temp[temp_idx], result, &result_idx);
		
		

		temp_idx++;
	}

}

char	**split_redir_with_quote(char **temp)
{
	char	**result;
	int		total_count;

	total_count = count_redir(temp);
	result = ft_calloc(total_count + 1, sizeof(char *));
	split_by_redir(temp, result);

	return (result);
}

