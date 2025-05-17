/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   implement.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 16:57:12 by woonkim           #+#    #+#             */
/*   Updated: 2025/05/17 12:30:01 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	child_work(t_object *object, t_imp_stus *imp_stus);
static void	parent_while_process(t_object *object, t_imp_stus *imp_stus);
static void	child_while_process(t_object *object, t_imp_stus *imp_stus);
static void implement2(t_object *object, t_imp_stus *imp_stus);


// 명령어와 환경 병수 링크드 리스크로 받음
// t_cmd : 명령어에 대한 meta data를 저장하고 있는 node
// env : 환경 변수를 저장하고 있는 node
// 명령어 없이 파이프라인만 있는 경우
void implement(t_object *object)
{
	t_imp_stus	imp_stus;
	int			one_builtin_flag;	
	t_cmd_info	*existing_cmd_info;

	existing_cmd_info = object->cmd_info;
	print_log(1, object, "\n---------------------- 구현부 -------------------------\n\n");
	// 명령어 갯수에 맞게 2차원 pipe 배열, child pid배열, child 종료 상태 배열 저장
	setting_pipline(object->cmd_info, &imp_stus);
	// builtin 명령어 하나일 때 true return
	one_builtin_flag = check_one_builtin(object, &imp_stus);
	if (one_builtin_flag)
	{	
		// execve에 건네줄 수 있는 argv를 만든다 (리다이렉션 전까지 짜름)
		create_execve_args(object->cmd_info);
		input_output_setting(object, &imp_stus, one_builtin_flag);
		execute_one_builtin(object, &imp_stus);
		free_stus(&imp_stus);
		print_log(1, object, "(부모에서 실행) complete one builtin execution\n");
		return ;
	}
	print_log(1, object, "total cmd : %d \n", imp_stus.total_c_n);
	implement2(object, &imp_stus);
	object->cmd_info = existing_cmd_info;
	return ;
}

static void implement2(t_object *object, t_imp_stus *imp_stus)
{
	// 부모 process부분, 명령어 반복 실행 (cur_c_n은 index로 쓰이기에 0부터 시작)
	while (imp_stus->cur_c_n < imp_stus->total_c_n)
	{
		// 파이프 생성 및 fork()하는 함수
		pipe_and_fork(imp_stus);
		if (imp_stus->chil_pid[imp_stus->cur_c_n] != 0)
			print_log(1, object, \
			"(부모에서 출력) pid %d : create child process ok\n", imp_stus->chil_pid[imp_stus->cur_c_n]);
		// 자식 process 실행 (실행파일 + builtin모두 처리)
		if (imp_stus->chil_pid[imp_stus->cur_c_n] == 0)
			child_while_process(object, imp_stus);
		// 부모 process 실행
		else
			parent_while_process(object, imp_stus);
	}
	// 자식 프로세스를 모두 생성한 후에 에러 출력 파이프 닫아줌
	close(imp_stus->stderr_pipe[1]);
	// 자식 프로세스 종료 대기 (비정상 종료시 상태 저장)
	wait_childs_process(object, imp_stus);
	// imp_stus 초기화 (부모 프로세스라 exit()실행하면 안됨)
	free_stus(imp_stus);
}

// fork후 자식 프로세스를 처리하는 함수
static void	child_while_process(t_object *object, t_imp_stus *imp_stus)
{
	// 자식 프로세스 시그널 처리
	init_child_signal();
	signal(SIGPIPE, SIG_IGN);
	// 실행파일 명령어라면 child_work()까지만 실행된다
	child_work(object, imp_stus);
	print_log(imp_stus->stdoutFd, object, "(builtin %d) ok (before exit())\n", (int)getpid());
	// safety_exit()이 실행되는 상황은 builtin일 때임
	// 자식 프로세스는 object, imp_stus 다 free하고 exit()해야 한다
	free_stus_and_object(object, imp_stus);
	exit(0);
}

// fork후 부모 프로세스를 처리하는 함수
static void	parent_while_process(t_object *object, t_imp_stus *imp_stus)
{
	// 부모 proecess는 쓰기 close
	close(imp_stus->pipeFd[imp_stus->cur_c_n][1]);
	// 이전 파이프 buffer 읽기 fd 닫기 (fork한 후 생성한 자식이 유지하고 있음)
	// 마지막 명령어에선 이전 buufer 파이프 읽기 fd 닫기
	if (object->cmd_info->prev != NULL)
		close(imp_stus->pipeFd[imp_stus->cur_c_n - 1][0]);
	if (object->cmd_info->next == NULL)
		close(imp_stus->pipeFd[imp_stus->cur_c_n][0]);
	// 명령어 index 및 node 이동
	imp_stus->cur_c_n += 1;
	object->cmd_info = object->cmd_info->next;
}

// 자식 process를 명령어에 맞게 execve하여 동작하게 함
// 자식 process의 input과 output fd를 dup2로 적절히 컨트롤 해야함
// 명령어가 builtin이라고 해도 fork()로 처리하는 것이 파이프라인 전체 구조를
// 설계할 때 더 편하다
static void child_work(t_object *object, t_imp_stus *imp_stus)
{
	char	**envp;
	int 	flag;

	// input, output을 재설정 해야 한다면 재설정
	input_output_setting(object, imp_stus, 0);
	// builtins check후 맞다면 builtin 실행
	if (execute_builtins(object, imp_stus))
		return ;
	// "", ''처리
	single_duuble_quates_check(object);
	// find_path로 찾은 path는 t_cmd의 cmd_path에 저장
	flag = find_path(object->cmd_info, object->env);
	// execve에 건네줄 수 있는 argv를 만든다
	create_execve_args(object->cmd_info);
	// env 링크드리스크 -> 2차원 배열 전환
	envp = env_to_char(object->env, imp_stus->i);
	print_log(imp_stus->stdoutFd, object, "(실행파일 %d) ok (before execve)\n", (int)getpid());
	if (!flag || execve(object->cmd_info->cmd_path,
			   object->cmd_info->evecve_argv, envp) == -1)
	{
		free_stus_and_object(object, imp_stus);
		free_doublechar(envp);
		exit(2);
	}
}
