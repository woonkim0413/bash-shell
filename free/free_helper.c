/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 13:58:34 by rakim             #+#    #+#             */
/*   Updated: 2025/05/18 00:07:36 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_stus(t_imp_stus *imp_stus)
{
	int	i;

	free(imp_stus->chil_pid);
	// 왜 close(imp_stus->stdoutFd);에서 segment fault가 뜨는거지?
	// -> stdoutFd를 close하여 이후엔 dprintf가 실행되지 못하는 것이지 여기서 error가 뜨는게 아님
	close(imp_stus->stdoutFd);
	close(imp_stus->stdinFd);
	// 
	i = 0;
	// 일기 파이프 fd close해준 뒤 free
	while (i < imp_stus->total_c_n)
	{
		// 파이프 안 만들고 close하면 pipeFd[i][0]에 담긴 쓰레기 값이
		// close된다 이때, 쓰레기 값은 컴파일러가 0으로 넣어줄 확률이 크기에 
		// STDIN이 닫혀서 main.c의 readline에 EOF가 전달되어 프로그램이 종료될 수 있다 
		free(imp_stus->pipeFd[i]);
		i ++;
	}
	free(imp_stus->pipeFd);
}

void	free_string_arr(char ***string_arr)
{
	int	idx;

	if (!string_arr || !(*string_arr))
		return ;
	idx = 0;
	while ((*string_arr)[idx])
	{
		free((*string_arr)[idx]);
		idx++;
	}
	free((*string_arr));
	(*string_arr) = NULL;
}

void	free_env(t_env **env)
{
	t_env	*temp;

	while ((*env))
	{
		temp = (*env)->next;
		if ((*env)->key)
			free((*env)->key);
		if ((*env)->value)
			free((*env)->value);
		free((*env));
		(*env) = temp;
	}
	(*env) = NULL;
}

void	free_redirect(t_redirect **redirect)
{
	t_redirect	*temp;

	while ((*redirect))
	{
		temp = (*redirect)->next;
		if ((*redirect)->file_path)
			free((*redirect)->file_path);
		free(*redirect);
		(*redirect) = temp;
	}
	(*redirect) = NULL;
}

void	free_cmd_info(t_cmd_info **cmd_info)
{
	t_cmd_info	*temp;

	while ((*cmd_info))
	{
		temp = (*cmd_info)->next;
		if ((*cmd_info)->cmd)
			free((*cmd_info)->cmd);
		if ((*cmd_info)->evecve_argv)
			free_string_arr(&((*cmd_info)->evecve_argv));
		if ((*cmd_info)->cmd_path)
			free((*cmd_info)->cmd_path);
		if ((*cmd_info)->redirect)
			free_redirect(&((*cmd_info)->redirect));
		free(*cmd_info);
		(*cmd_info) = temp;
	}
	(*cmd_info) = NULL;
}
