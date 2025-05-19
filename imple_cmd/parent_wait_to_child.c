/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent_wait_to_child.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 20:07:27 by woonkim           #+#    #+#             */
/*   Updated: 2025/05/19 10:32:11 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void error_process(t_object *object, t_imp_stus *imp_stus, pid_t ret);

// 모든 자식 프로세스 종료 대기 (비정상 종료시 상태 저장)
// 비정상 종료하는 경우 여기서 error메세지를 terminal로 출력
void wait_childs_process(t_object *object, t_imp_stus *imp_stus)
{
	pid_t	ret;
	int		raw_exit_stus;

	raw_exit_stus = 0;
	while (imp_stus->i < imp_stus->total_c_n)
	{
		ret = waitpid(-1, &raw_exit_stus, 0);
		error_process(object, imp_stus, ret);	
		if (ret < 0)
			perror("waitpid error : ");
		if (WIFSIGNALED(raw_exit_stus))
		{
			object->last_exit_status = 0;
			print_log(imp_stus->stdoutFd, object, \
				"%d (signal exit)\n", object->last_exit_status);
		}
		if (WIFEXITED(raw_exit_stus))
			object->last_exit_status = WEXITSTATUS(raw_exit_stus);
		imp_stus->i += 1;
		print_log(imp_stus->stdoutFd, object, \
			"(부모에서 출력) pid %d : child process exit \n", (int)ret);
	}
	close(imp_stus->stderr_pipe[0]);
}

// execve에서 발생한 pipe broken인 경우 (ex: echo asd | cat < file1.txt)
static void	error_process(t_object *object, t_imp_stus *imp_stus, pid_t ret)
{
	char	buf[4096];
	int		flag;

	(void)object;
	(void)ret;
	flag = read(imp_stus->stderr_pipe[0], buf, sizeof(buf) - 1);
	if (flag <= 0)
	{
		return ;
	}
	buf[flag] = '\0';
	if (ft_strnstr(buf, "Broken pipe", sizeof(buf) - 1))
	{
		print_log(imp_stus->stdoutFd, object, "(%d)(임시 출력) %s\n", (int)ret ,buf);
		return ;
	}
	if (ft_strnstr(buf, "write error", sizeof(buf) - 1))
	{
		print_log(imp_stus->stdoutFd, object, "(%d)(임시 출력) %s\n", (int)ret ,buf);
		return ;
	}
	printf("Error : %s", buf);
}
