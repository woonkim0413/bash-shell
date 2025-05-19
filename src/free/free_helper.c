/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 13:58:34 by rakim             #+#    #+#             */
/*   Updated: 2025/05/19 14:24:26 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_stus(t_imp_stus *imp_stus)
{
	int	i;

	free(imp_stus->chil_pid);
	close(imp_stus->stdout_fd);
	close(imp_stus->stdin_fd);
	i = 0;
	while (i < imp_stus->total_c_n)
	{
		free(imp_stus->pipe_fd[i]);
		i ++;
	}
	free(imp_stus->pipe_fd);
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
			free_string(&(*cmd_info)->cmd);
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
