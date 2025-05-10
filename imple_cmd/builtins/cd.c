/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 13:50:45 by woonkim           #+#    #+#             */
/*   Updated: 2025/05/05 21:55:31 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void update_env(t_object *object, t_imp_stus *imp_stus);

// PWD 위치를 변경해주는 명령어다 
int execute_cd(t_object *object, t_imp_stus *imp_stus)
{
	int		flag;
	char 	*path;

	path = object->cmd_info->evecve_argv[1];
	if (path == NULL)
		return (1);
	flag = chdir(path);
	if (flag == -1)
	{
		write(imp_stus->stdoutFd, "bash: cd: ", 10);
		write(imp_stus->stdoutFd, path, ft_strlen(path));
		write(imp_stus->stdoutFd,": No Such file or directory\n", 29);
	}
	else
	{
		update_env(object, imp_stus);
	}
	return (1);
}

// 위치가 정상적으로 변경된 경우 PWD 환경변수 값을 변경하는 함수
// getcwd로 현재 위치 획득 -> 환경변수 arg값 생성 -> export 실행
// (export는 내부적으로 이미 환경 변수가 있다면 value를 update함)
static void update_env(t_object *object, t_imp_stus *imp_stus)
{
	char *export_arg;

	char buffer[4096];
	getcwd(buffer, sizeof(buffer) - 1);
	export_arg = ft_strjoin("PWD=", buffer);
	free(object->cmd_info->evecve_argv[1]);
	object->cmd_info->evecve_argv[1] = export_arg;
	execute_export(object, imp_stus);
}
