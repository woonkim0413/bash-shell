/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 16:00:12 by rakim             #+#    #+#             */
/*   Updated: 2025/04/21 14:42:14 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	void	free_all(t_object *object)
{
	t_env		*env;
	t_env		*env_temp;
	t_cmd_info	*cmd_info;
	t_cmd_info	*cmd_info_temp;

	env = object->env;
	cmd_info = object->cmd_info;
	while (env)
	{
		env_temp = env->next;
		if (env->key)
			free(env->key);
		if (env->value)
			free(env->value);
		free(env);
		env = env_temp;
	}
	while (cmd_info)
	{
		cmd_info_temp = cmd_info->next;
		free(cmd_info->cmd);
	}
}

void	throw_error(char *message, t_object *object)
{
	if (object)
		free_all(object);
	printf("%s\n", message);
	exit(1);
}
