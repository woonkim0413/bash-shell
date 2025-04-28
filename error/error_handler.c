/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 16:00:12 by rakim             #+#    #+#             */
/*   Updated: 2025/04/28 19:23:46 by woonkim          ###   ########.fr       */
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


static void free_stus(t_imp_stus *imp_stus, int flag)
{
	int i;

	free(imp_stus->chil_pid);
	close(imp_stus->stdoutFd);
	i = 0;
	// 일기 파이프 fd close해준 뒤 free
	while (i < imp_stus->total_c_n)
	{
		// 파이프 안 만들고 close하면 pipeFd[i][0]에 담긴 쓰레기 값이
		// close된다 이때, 쓰레기 값은 컴파일러가 0으로 넣어줄 확률이 크기에 
		// STDIN이 닫혀서 main.c의 readline에 EOF가 전달되어 종료된 것이었다 
		if (!flag)
			close(imp_stus->pipeFd[i][0]);
		free(imp_stus->pipeFd[i]);
		i ++;
	}
	free(imp_stus->pipeFd);
}

void	throw_error(char *message, t_object *object, t_imp_stus *imp_stus)
{
	if (object)
		free_all(object);
	if (imp_stus)
		free_stus(imp_stus, 0);
	printf("Error : %s\n", message);
	exit(1);
}

// error_handler 사용하기 :
// message에 NULL, imp_stus 객체 추가
// flag에 1을 넣으면 free_stus에서 close하지 않는다
void	safety_exit(t_object *object, t_imp_stus *imp_stus, int flag)
{
	if (object)
		(void)object;
	// free_all(object);
	if (imp_stus)
		free_stus(imp_stus, flag);
	// exit(0);
}
