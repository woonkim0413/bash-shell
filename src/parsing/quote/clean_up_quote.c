/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up_quote.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 14:41:03 by rakim             #+#    #+#             */
/*   Updated: 2025/05/13 16:20:26 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	char	*remove_outer_quote(char **str)
{
	char	*result;
	int		in_single;
	int		in_double;
	int		str_idx;
	int		result_idx;

	in_single = 0;
	in_double = 0;
	str_idx = 0;
	result_idx = 0;
	result = ft_calloc(ft_strlen(*str) + 1, sizeof(char));
	while ((*str)[str_idx])
	{
		if ((*str)[str_idx] == '\'' && !in_double)
			in_single = !in_single;
		else if ((*str)[str_idx] == '"' && !in_single)
			in_double = !in_double;
		else
			result[result_idx++] = (*str)[str_idx];
		str_idx++;
	}
	free(*str);
	return (result);
}

static	void	clean_up_redirect(t_cmd_info *cmd_info)
{
	t_redirect	*crr_redir;

	crr_redir = cmd_info->redirect;
	while (crr_redir)
	{
		if (crr_redir->file_path)
			crr_redir->file_path = remove_outer_quote(&(crr_redir->file_path));
		crr_redir = crr_redir->next;
	}
}

void	clean_up_quote(t_cmd_info *cmd_info)
{
	t_cmd_info	*crr_cmd;
	int			idx;

	if (!cmd_info)
		return ;
	crr_cmd = cmd_info;
	while (crr_cmd)
	{
		if (crr_cmd->cmd)
			crr_cmd->cmd = \
			remove_outer_quote(&(crr_cmd->cmd));
		if (crr_cmd->evecve_argv)
		{
			idx = 0;
			while (crr_cmd->evecve_argv[idx])
			{
				crr_cmd->evecve_argv[idx] = \
				remove_outer_quote(&(crr_cmd->evecve_argv[idx]));
				idx++;
			}
		}
		clean_up_redirect(crr_cmd);
		crr_cmd = crr_cmd->next;
	}
}
