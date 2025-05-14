/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 16:00:12 by rakim             #+#    #+#             */
/*   Updated: 2025/05/13 20:56:45 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	throw_error(char *message, t_object *object, \
	t_imp_stus *imp_stus, char **line)
{
	if (object && object->cmd_info)
		free_cmd_info(&(object->cmd_info));
	if (imp_stus)
		free_stus(imp_stus);
	if (line)
	{
		free((*line));
		(*line) = NULL;
	}
	printf("Error : %s\n", message);
	if (!ft_strncmp("malloc error", message, ft_strlen(message)))
		exit(1);
	if (!ft_strncmp("pipe_error", message, ft_strlen(message)))
		exit(1);
}
