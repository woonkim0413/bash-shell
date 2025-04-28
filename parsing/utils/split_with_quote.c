/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_with_quote.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 20:07:38 by rakim             #+#    #+#             */
/*   Updated: 2025/04/28 19:50:11 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// ' ', >, <, >>, <<
// cat asd>asd<asd "asd > asd< " > asd< asd                 asd | ls -al asd >asdd< | asd
// cat
// asd>asd<asd
// "asd > asd< "
// >
// asd<
// asd
// asd
// cat asd > asd < asd = 6
// cat "asd > asd" > asd = 4
// cat 'asd > asd' <asd = 4

/*
static int	count_word(char *s)
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
		if (!in_word && s[idx] && s[idx] != ' ')
		{
			in_word = 1;
			count++;
		}
		if (!in_single && !in_double)
		{
			if (s[idx] == ' ')
				in_word = 0;
			if ((s[idx] == '<' && s[idx + 1] == '<') || \
			(s[idx] == '>' && s[idx + 1] == '>'))
				in_word = 0;
			else if (s[idx] == '<' || s[idx] == '>')
				in_word = 0;
		}
		idx++;
	}
	return (count);
}
static	void	process_set_result(t_for_split_with_quote *arg)
{
	int		start;
	int		in_single;
	int		in_double;

	start = -1;
	in_single = 0;
	in_double = 0;
	while (arg->s[arg->idx])
	{
		set_toggle(arg->s[arg->idx], &in_single, &in_double);
		if (!in_single && !in_double)
			set_result(arg, &start);
		if ((in_single || in_double) && start == -1)
			start = arg->idx;
		if (!in_single && !in_double && start == -1)
			start = arg->idx;
		arg->idx++;
	}
	if (start != -1)
		(arg->result)[arg->word_idx++] = alloc_word(arg->s, start, arg->idx);
}
*/

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

static	char	**split_space_with_quote(char *s)
{
	int		word_count;
	char	**temp;
	char	**result;

	word_count = count_word_split_by_space(s);
	temp = ft_calloc(word_count + 1, sizeof(char *));
	split_by_space(&temp, s);
	result = split_redir_with_quote(temp);
	return (result);
}

char	**split_with_quote(char *s)
{
	char					**result;

	if (!s)
		return (NULL);
	result = split_space_with_quote(s);
	
	/*
	result = ft_calloc(count_word(s) + 1, sizeof(char *));
	if (!result)
		return (NULL);
	arg.result = result;
	arg.s = s;
	arg.idx = 0;
	arg.word_idx = 0;
	process_set_result(&arg);
	*/
	return (result);
}
