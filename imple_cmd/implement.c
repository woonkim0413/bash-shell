/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   implement.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 16:57:12 by woonkim           #+#    #+#             */
/*   Updated: 2025/05/04 17:28:42 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void setting_pipline(t_cmd_info *t_cmd, t_imp_stus *imp_stus);
static void child_work(t_object *object, t_imp_stus *imp_stus);
static void pipe_and_fork(t_imp_stus *imp_stus);

// 명령어와 환경 병수 링크드 리스크로 받음
// t_cmd : 명령어에 대한 meta data를 저장하고 있는 node
// env : 환경 변수를 저장하고 있는 node
// 명령어 없이 파이프라인만 있는 경우
void implement(t_object *object)
{
	t_imp_stus imp_stus;

	printf("\n---------------------- 구현부 -------------------------\n\n");
	// 명령어 갯수에 맞게 2차원 pipe 배열, child pid배열, child 종료 상태 배열 저장
	setting_pipline(object->cmd_info, &imp_stus);
	// builtin 명령어 하나일 때 실행
	if (one_builtin_case(object, &imp_stus))
	{
		free_stus(&imp_stus);
		printf("(부모에서 실행) complete one builtin execution\n");
		return ;
	}
	printf("total cmd : %d \n", imp_stus.total_c_n);
	// 부모 process부분, 명령어 반복 실행 (cur_c_n은 index로 쓰이기에 0부터 시작)
	while (imp_stus.cur_c_n < imp_stus.total_c_n)
	{
		// 파이프 생성 및 fork()하는 함수
		pipe_and_fork(&imp_stus);
		if (imp_stus.chil_pid[imp_stus.cur_c_n] != 0)
			printf("(부모에서 출력) pid %d : create child process ok\n", imp_stus.chil_pid[imp_stus.cur_c_n]);
		// 자식 process 실행 (실행파일 + builtin모두 처리)
		if (imp_stus.chil_pid[imp_stus.cur_c_n] == 0)
		{
			// 자식 프로세스 시그널 처리
			init_child_signal();
			signal(SIGPIPE, SIG_IGN);
			// 실행파일 명령어라면 child_work()까지만 실행된다
			child_work(object, &imp_stus);
			dprintf(imp_stus.stdoutFd, "(builtin %d) ok (before exit())\n", (int)getpid());
			// safety_exit()이 실행되는 상황은 builtin일 때임
			// 자식 프로세스는 object, imp_stus 다 free하고 exit()해야 한다
			free_stus_and_object(object, &imp_stus);
			exit(0);
		}
		// 부모 process 실행
		else
		{
			// 부모 proecess는 쓰기 close
			close(imp_stus.pipeFd[imp_stus.cur_c_n][1]);
			// 이전 파이프 buffer 읽기 fd 닫기 (fork한 후 생성한 자식이 유지하고 있음)
			// 마지막 명령어에선 이전 buufer 파이프 읽기 fd 닫기
			if (object->cmd_info->prev != NULL)
				close(imp_stus.pipeFd[imp_stus.cur_c_n - 1][0]);
			if (object->cmd_info->next == NULL)
				close(imp_stus.pipeFd[imp_stus.cur_c_n][0]);
			// 명령어 index 및 node 이동
			imp_stus.cur_c_n += 1;
			object->cmd_info = object->cmd_info->next;
		}
	}
	// 자식 프로세스를 모두 생성한 후에 에러 출력 파이프 닫아줌
	close(imp_stus.stderr_pipe[1]);
	// 자식 프로세스 종료 대기 (비정상 종료시 상태 저장)
	wait_childs_process(object, &imp_stus);
	// imp_stus 초기화 (부모 프로세스라 exit()실행하면 안됨)
	free_stus(&imp_stus);
	printf("----- all complete ------\n");
	return ;
}

// 파이프 생성 및 fork()하는 함수
static void pipe_and_fork(t_imp_stus *imp_stus)
{
	// 현재 명령어 index에 위치한 pipFd배열로 pipe buffer생성
	pipe(imp_stus->pipeFd[imp_stus->cur_c_n]);
	imp_stus->chil_pid[imp_stus->cur_c_n] = fork();
}

// 자식 process를 명령어에 맞게 execve하여 동작하게 함
// 자식 process의 input과 output fd를 dup2로 적절히 컨트롤 해야함
// 명령어가 builtin이라고 해도 fork()로 처리하는 것이 파이프라인 전체 구조를
// 설계할 때 더 편하다
static void child_work(t_object *object, t_imp_stus *imp_stus)
{
	char	**envp;
	int		flag;

	// env 링크드리스크 -> 2차원 배열 전환
	envp = env_to_char(object->env);
	// input, output을 재설정 해야 한다면 재설정
	input_output_setting(object, imp_stus);
	// cmd가 널인 경우 check
	flag = cmd_null_check(object, imp_stus);
	if (flag)
		return ;
	// execve에 건네줄 수 있는 argv를 만든다
	create_execve_args(object->cmd_info);
	// -------------------출력 테스트 시작 ---------------------
	// dprintf(imp_stus->stdoutFd, "-----출력 테스트 시작 ------\n");
	// int i = -1;
	// dprintf(imp_stus->stdoutFd, "(child %d) argv 출력 :", (int)getpid());
	// while (object->cmd_info->evecve_argv[++i])
	// 	dprintf(imp_stus->stdoutFd, "(%s) ", object->cmd_info->evecve_argv[i]);
	// dprintf(imp_stus->stdoutFd, "\n");
	// dprintf(imp_stus->stdoutFd, "-----출력 테스트 끝 ------\n");
	// -------------------출력 테스트 끝 -----------------------
	// builtins check후 맞다면 builtin 실행
	if (execute_builtins(object, imp_stus))
	{
		return ;
	}
	// find_path로 찾은 path는 t_cmd의 cmd_path에 저장
	// TODO : /bin/ls와 같은 절대 주소가 전달된 경우 처리
	find_path(object->cmd_info, object->env);
	dprintf(imp_stus->stdoutFd, "(실행파일 %d) ok (before execve)\n", (int)getpid());
	if (execve(object->cmd_info->cmd_path,
			   object->cmd_info->evecve_argv, envp) == -1)
		throw_error("FAILD EXECVE", object, imp_stus);
}

// t_cmd를 받아서 명령어 숫자를 세고 그에 맞게 pipeFd 배열 생성
// pipFd는 각 index마다 fd closs하고 index free하고 pipFd free해야 함
static void setting_pipline(t_cmd_info *t_cmd, t_imp_stus *imp_stus)
{
	int i;
	int num;

	// imp_stus 초기화
	init_t_imp_stus(imp_stus);
	num = 0;
	imp_stus->stdoutFd = dup(STDOUT_FILENO);
	// 에러 처리 버퍼
	pipe(imp_stus->stderr_pipe);
	while (t_cmd)
	{
		num++;
		t_cmd = t_cmd->next;
	}
	// 총 명령어 갯수 저장
	imp_stus->total_c_n = num;
	// child pid 배열 저장
	imp_stus->chil_pid = (pid_t *)malloc(sizeof(pid_t) * num);
	// pipFd[2]배열 저장
	imp_stus->pipeFd = (int **)malloc(sizeof(int *) * num);
	i = 0;
	while (i < num)
		imp_stus->pipeFd[i ++] = (int *)malloc(sizeof(int) * 2);
}
