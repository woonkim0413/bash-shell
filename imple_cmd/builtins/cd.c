/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 13:50:45 by woonkim           #+#    #+#             */
/*   Updated: 2025/05/04 23:06:51 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TODO : cd명령어 사용에 따른 환경변수 파일의 CWD 값 변경
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

// pwd : update된 위치 획득
// unset : CWD 제거
// export : CWD update 
void update_env(t_object *object, t_imp_stus *imp_stus)
{
	
}
