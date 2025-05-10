/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 15:47:06 by woonkim           #+#    #+#             */
/*   Updated: 2025/05/09 16:30:28 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 하나의 명령어만 사용 + 하나의 명령어가 builtin인 경우 실행
int	check_one_builtin(t_object *object, t_imp_stus *imp_stus)
{
	char *cmd;
	int flag;

	(void)imp_stus;
	flag = 0;
	if (object->cmd_info->prev == NULL && object->cmd_info->next == NULL)
	{
		cmd = object->cmd_info->cmd;
		if (!ft_strncmp("echo", cmd, ft_strlen(cmd)))
			flag = 1;
		else if (!ft_strncmp("pwd", cmd, ft_strlen(cmd)))
			flag = 1;
		else if (!ft_strncmp("unset", cmd, ft_strlen(cmd)))
			flag = 1;
		else if (!ft_strncmp("export", cmd, ft_strlen(cmd)))
			flag = 1;
		else if (!ft_strncmp("env", cmd, ft_strlen(cmd)))
			flag = 1;
		else if (!ft_strncmp("exit", cmd, ft_strlen(cmd)))
			flag = 1;
		else if (!ft_strncmp("cd", cmd, ft_strlen(cmd)))
			flag = 1;
	}
	return (flag);
}

//  파이프라인에서 builtin을 사용할 때 실행됨 ; fork()후 자식 프로세스에서 실행됨
int	execute_builtins(t_object *object, t_imp_stus *imp_stus)
{
	int flag;
	char *cmd;

	flag = 0;
	cmd = object->cmd_info->cmd;
	if (!ft_strncmp("echo", cmd, ft_strlen(cmd)))
		flag = execute_echo(object, imp_stus);
	else if (!ft_strncmp("pwd", cmd, ft_strlen(cmd)))
		flag = execute_pwd(object, imp_stus, -1);
	else if (!ft_strncmp("unset", cmd, ft_strlen(cmd)))
		flag = execute_unset(object, imp_stus);
	else if (!ft_strncmp("export", cmd, ft_strlen(cmd)))
		flag = execute_export(object, imp_stus);
	else if (!ft_strncmp("env", cmd, ft_strlen(cmd)))
		flag = execute_env(object, imp_stus);
	else if (!ft_strncmp("exit", cmd, ft_strlen(cmd)))
		flag = execute_exit(object, imp_stus);
	else if (!ft_strncmp("cd", cmd, ft_strlen(cmd)))
		flag = execute_cd(object, imp_stus);
	return (flag);
}

// * builtin 단독 사용시 실행됨 ; 부모 프로세스에서 실행됨
void	execute_one_builtin(t_object *object, t_imp_stus *imp_stus)
{
	char *cmd;

	cmd = object->cmd_info->cmd;
	if (!ft_strncmp("echo", cmd, ft_strlen(cmd)))
		execute_echo(object, imp_stus);
	else if (!ft_strncmp("pwd", cmd, ft_strlen(cmd)))
		 execute_pwd(object, imp_stus, -1);
	else if (!ft_strncmp("unset", cmd, ft_strlen(cmd)))
		execute_unset(object, imp_stus);
	else if (!ft_strncmp("export", cmd, ft_strlen(cmd)))
		execute_export(object, imp_stus);
	else if (!ft_strncmp("env", cmd, ft_strlen(cmd)))
		execute_env(object, imp_stus);
	else if (!ft_strncmp("exit", cmd, ft_strlen(cmd)))
		execute_exit(object, imp_stus);
	else if (!ft_strncmp("cd", cmd, ft_strlen(cmd)))
		execute_cd(object, imp_stus);
	// fd 연결 재설정
	dup2(imp_stus->stdoutFd, STDOUT_FILENO);
	dup2(imp_stus->stdinFd, STDIN_FILENO);
}