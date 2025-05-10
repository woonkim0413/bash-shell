/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent_wait_to_child.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 20:07:27 by woonkim           #+#    #+#             */
/*   Updated: 2025/05/09 16:32:37 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void error_process(t_object *object, t_imp_stus *imp_stus, pid_t ret);

// 모든 자식 프로세스 종료 대기 (비정상 종료시 상태 저장)
// 비정상 종료하는 경우 여기서 error메세지를 terminal로 출력
void wait_childs_process(t_object *object, t_imp_stus *imp_stus)
{
	pid_t ret;
	int code;

	while (imp_stus->i < imp_stus->total_c_n)
	{
		// printf("%d번 명령어 : before waitpid()\n", imp_stus->i);
		// 특정 자식 process가 종료될 때까지 대기, 비정상 종료시 flag저장
		ret = waitpid(-1, &(object->last_exit_status), 0);
		// printf("%d번 명령어 : after waitpid()\n", imp_stus->i);
		if (ret < 0)
			perror("waitpid error : ");
		//시그널 종료 확인
		if (WIFSIGNALED(object->last_exit_status))
			printf("%d 시그널 종료\n", WTERMSIG(object->last_exit_status));
		// 자식 프로세스가 정상 종료됐는지 확인 (exit(), return 종료)
		if (WIFEXITED(object->last_exit_status))
		{
			// 종료 상태 값 확인
			code = WEXITSTATUS(object->last_exit_status);
			if (code)
			{
				printf("(%d)(임시출력) EXIT number : %d\n", (int)ret, WEXITSTATUS(object->last_exit_status));
				error_process(object, imp_stus, ret);	
			}
		}
		imp_stus->i += 1;
		printf("(부모에서 출력) pid %d : child process exit \n", (int)ret);
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
