/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 16:00:12 by rakim             #+#    #+#             */
/*   Updated: 2025/04/27 14:56:06 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_all(t_object *object)
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
	cmd_info_temp = cmd_info;
	while (cmd_info_temp)
	{
		if (cmd_info->evecve_argv)
			free_string_arr(cmd_info->evecve_argv);
		// redirect도 날려야 함
		if (cmd_info->cmd)
			free(cmd_info->cmd);
		cmd_info_temp = cmd_info_temp->next;
	}
}

void	throw_error(char *message, t_object *object)
{
	if (object)
		free_all(object);
	printf("Error : %s\n", message);
	exit(1);
}
