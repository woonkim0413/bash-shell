/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   imp_utils3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 14:38:18 by woonkim           #+#    #+#             */
/*   Updated: 2025/05/04 13:36:20 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int one_builtin_case(t_object *object, t_imp_stus *imp_stus)
{
	char *cmd;
	int flag;

	flag = 0;
	if (object->cmd_info->prev == NULL && object->cmd_info->next == NULL)
	{
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
	}
	return (flag);
}

void free_doublechar(char **argv)
{
	int i;

	i = 0;
	while (argv[i])
		free(argv[i++]);
	free(argv);
}

// execve에서 발생한 pipe broken인 경우 (ex: echo asd | cat < file1.txt)
void error_process(t_object *object, t_imp_stus *imp_stus, pid_t ret)
{
	char buf[4096];
	int	flag;

	(void)object;
	(void)ret;
	// 자식 프로세스가 0아닌 EXIT으로 종료해도 error 메세지는 없을 수 있음
	flag = read(imp_stus->stderr_pipe[0], buf, sizeof(buf) - 1);
	if (flag == 0)
	{
		printf("(읽어올 에러 문자 없음)\n");
		return ;
	}
	// 특정 error출력 씹음
	if (ft_strnstr(buf, "Broken pipe", sizeof(buf) - 1))
	{
		// printf("(%d)(임시 출력) %s\n", (int)ret ,buf);
		return ;
	}
	printf("(정상 에러 출력 : ) %s\n", buf);
}

int cmd_null_check(t_object *object,t_imp_stus *imp_stus)
{
	(void)imp_stus;
	if (object->cmd_info->cmd)
		return (0);
	return (1);
}
