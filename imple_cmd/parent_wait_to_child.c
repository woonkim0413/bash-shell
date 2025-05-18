/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent_wait_to_child.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 20:07:27 by woonkim           #+#    #+#             */
/*   Updated: 2025/05/18 16:55:57 by woonkim          ###   ########.fr       */
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
		// 특정 자식 process가 종료될 때까지 대기, 비정상 종료시 flag저장
		ret = waitpid(-1, &raw_exit_stus, 0);
		// 에러 출력
		error_process(object, imp_stus, ret);	
		if (ret < 0)
			perror("waitpid error : ");
		//시그널 종료 확인
		if (WIFSIGNALED(raw_exit_stus))
		{
			// < file1.txt와 같은 시그널 종료시 정상 상태 넣어주기기 
			object->last_exit_status = 0;
			print_log(imp_stus->stdoutFd, object, "%d (signal exit)\n", object->last_exit_status);
		}
		// 자식 프로세스가 정상 종료됐는지 확인 (exit(), return 종료)
		if (WIFEXITED(raw_exit_stus))
		{
			// 종료 상태 값 확인
			object->last_exit_status = WEXITSTATUS(raw_exit_stus);
			if (object->last_exit_status)
				print_log(imp_stus->stdoutFd, object, "(%d)(임시 출력) Error EXIT number : %d\n", (int)ret, object->last_exit_status);
		}
		imp_stus->i += 1;
		print_log(imp_stus->stdoutFd, object, "(부모에서 출력) pid %d : child process exit \n", (int)ret);
	}
	// 에러 출력 후에 읽기 파이프도 닫아줌
	close(imp_stus->stderr_pipe[0]);
}

// execve에서 발생한 pipe broken인 경우 (ex: echo asd | cat < file1.txt)
static void error_process(t_object *object, t_imp_stus *imp_stus, pid_t ret)
{
	char buf[4096];
	int	flag;

	(void)object;
	(void)ret;
	// 자식 프로세스가 0아닌 EXIT으로 종료해도 error 메세지는 없을 수 있음
	flag = read(imp_stus->stderr_pipe[0], buf, sizeof(buf) - 1);
	if (flag <= 0)
	{
		return ;
	}
	buf[flag] = '\0'; 
	// 특정 error출력 씹음
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
