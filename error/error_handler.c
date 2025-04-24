/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 16:00:12 by rakim             #+#    #+#             */
/*   Updated: 2025/04/27 21:06:59 by woonkim          ###   ########.fr       */
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

static void free_stus(t_imp_stus *imp_stus)
{
	int i;

	free(imp_stus->chil_pid);
	free(imp_stus->chil_e_stus);
	close(imp_stus->stdoutFd);
	i = 0;
	// 일기 파이프 fd close해준 뒤 free
	while (i < imp_stus->total_c_n)
	{
		close(imp_stus->pipeFd[i][0]);
		free(imp_stus->pipeFd[i]);
		i ++;
	}
	free(imp_stus->pipeFd);
}

// error_handler 사용하기 :
// message에 NULL, imp_stus 객체 추가
void	safety_exit(t_object *object, t_imp_stus *imp_stus)
{
	if (object)
		free_all(object);
	if (imp_stus)
		free_stus(imp_stus);
	exit(0);
}
