/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 14:45:46 by rakim             #+#    #+#             */
/*   Updated: 2025/05/19 14:49:04 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_heardoc(t_object *object)
{
	t_cmd_info	*cmd_info;
	t_redirect	*redirect;

	if (!object->cmd_info)
		return ;
	cmd_info = object->cmd_info;
	while (cmd_info)
	{
		redirect = cmd_info->redirect;
		while (redirect)
		{
			if (redirect->type == TOKEN_HEREDOC)
				write_heardoc_in_pipe(object, redirect);
			if (!object->cmd_info)
				return ;
			redirect = redirect->next;
		}
		cmd_info = cmd_info->next;
	}
}
