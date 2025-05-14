/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 14:14:29 by rakim             #+#    #+#             */
/*   Updated: 2025/05/13 16:12:57 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_object(t_object *object)
{
	free_env(&(object->env));
	free_cmd_info(&(object->cmd_info));
}

void	free_stus_and_object(t_object *object, t_imp_stus *imp_stus)
{
	if (object)
		free_object(object);
	if (imp_stus)
		free_stus(imp_stus);
}

void	free_string(char **line)
{
	free((*line));
	(*line) = NULL;
}

void	free_arg(t_check_redir_arg *arg)
{
	if (arg->src)
		free_string_arr(&(arg->src));
	if (arg->cmd)
		free_string(&(arg->cmd));
	if (arg->redirect)
		free_redirect(&(arg->redirect));
}
