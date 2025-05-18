/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_by_redir_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 14:39:33 by rakim             #+#    #+#             */
/*   Updated: 2025/05/15 14:36:59 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	int	set_result(char *s, int *s_idx, \
	t_result_info *result_info, int *start)
{
	char	*file_path;

	if (result_info->file_path_flag == 1)
	{
		file_path = ft_substr(s, *start, *s_idx - *start);
		if (!(*file_path))
			free(file_path);
		else
		{
			result_info->result[(result_info->result_idx)++] = file_path;
			result_info->file_path_flag = 0;
		}
	}
	result_info->result[(result_info->result_idx)++] = \
	ft_substr(s, *s_idx, 1 + result_info->add_value);
	(*start) = -1;
	(*s_idx) += result_info->add_value;
	result_info->file_path_flag = 1;
	return (1);
}

static	int	split_when_redir(char *s, int *s_idx, \
	t_result_info *result_info, int *start)
{
	if ((s[*s_idx] == '>' && s[(*s_idx) + 1] == '>') || \
	(s[*s_idx] == '<' && s[(*s_idx) + 1] == '<'))
	{
		result_info->add_value = 1;
		return (set_result(s, s_idx, result_info, start));
	}
	else if (s[*s_idx] == '>' || s[*s_idx] == '<')
	{
		result_info->add_value = 0;
		return (set_result(s, s_idx, result_info, start));
	}
	return (0);
}

static	void	process_split_by_redir(char *s, t_result_info *result_info)
{
	int		s_idx;
	int		in_single;
	int		in_double;
	int		start;


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
			split_when_redir(s, &s_idx, result_info, &start);
		s_idx++;
	}
	if (start != -1)
	{
		result_info->result[(result_info->result_idx)++] = \
		ft_substr(s, start, s_idx - start);
		if (result_info->file_path_flag == 1)
			result_info->file_path_flag = 0;
	}
}

void	split_by_redir(char **temp, t_result_info *result_info)
{
	int	temp_idx;

	temp_idx = 0;
	result_info->file_path_flag = 0;
	while (temp[temp_idx])
	{
		process_split_by_redir(temp[temp_idx], result_info);
		temp_idx++;
	}
}
