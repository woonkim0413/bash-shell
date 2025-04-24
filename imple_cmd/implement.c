/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   implement.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 16:57:12 by woonkim           #+#    #+#             */
/*   Updated: 2025/04/27 21:06:08 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void setting_pipline(t_cmd_info *t_cmd, t_imp_stus *imp_stus);
static void child_work(t_object *object, t_imp_stus *imp_stus);

// 명령어와 환경 병수 링크드 리스크로 받음
// t_cmd : 명령어에 대한 meta data를 저장하고 있는 node
// env : 환경 변수를 저장하고 있는 node
// 명령어가 없는 경우엔 cmd에 NULL (명령어가 없는 경우는 없음)
void implement(t_object *object)
{
	t_imp_stus imp_stus;

	// 명령어 갯수에 맞게 2차원 pipe 배열, child pid배열, child 종료 상태 배열 저장
	setting_pipline(object->cmd_info, &imp_stus);
	// 부모 process부분, 명령어 반복 실행 (cur_c_n은 index로 쓰이기에 0부터 시작)
	while (imp_stus.cur_c_n < imp_stus.total_c_n)
	{
		// 파이프 생성 및 fork()하는 함수
		pipe_and_fork(&imp_stus);
		// 자식 process 실행 (실행파일 + builtin모두 처리)
		if (imp_stus.chil_pid[imp_stus.cur_c_n] == 0)
			child_work(object, &imp_stus);
		// 부모 process 실행
		else
		{
			// 부모 proecess는 쓰기 close
			close(imp_stus.pipeFd[imp_stus.cur_c_n][1]);
			// 명령어 index 및 node 이동
			imp_stus.cur_c_n += 1;
			object->cmd_info = object->cmd_info->next;
		}
	}
	// 자식 프로세스 종료 대기 (비정상 종료시 상태 저장)
	wait_childs_process(&imp_stus, &imp_stus);
	// TODO : pipe배열 모두 close및 free해주기
	safety_exit(object, &imp_stus);
}

// 파이프 생성 및 fork()하는 함수
pipe_and_fork(t_imp_stus *imp_stus)
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
	char **envp;

	// 읽기 fd close, 자식을 쓰기만 함
	close(imp_stus->pipeFd[imp_stus->cur_c_n][0]);
	// env 링크드리스크 -> 2차원 배열 전환
	envp = env_to_char(object->env);
	// input, output을 재설정 해야 한다면 재설정
	input_output_connect(object, &imp_stus);
	// builtins check후 맞다면 builtin 실행
	execute_buitins(object, &imp_stus);
	// inputFd가 있다면 input을 stdin으로 받도록 설정
	if (object->cmd_info->input_fd != -1)
	{
		dup2(object->cmd_info->input_fd, 0);
		close(object->cmd_info->input_fd);
	}
	// output은 pipeFd를 통해 부모 process에 전달
	dup2(imp_stus->pipeFd[0][1], 1);
	close(imp_stus->pipeFd[0][1]);
	// find_path로 찾은 path는 t_cmd의 cmd_path에 저장
	find_path(object->cmd_info, object->env);
	// 명령어가 없는 경우 free
	if (object->cmd_info->cmd_path == NULL)
		throw_error("NOT FOUND CMD", object);
	if (execve(object->cmd_info->cmd_path,
			   object->cmd_info->evecve_argv, envp) == -1)
		throw_error("FAILD EXECVE", object);
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
	while (t_cmd)
	{
		num++;
		t_cmd = t_cmd->next;
	}
	// 총 명령어 갯수 저장
	imp_stus->total_c_n = num;
	// child pid 배열 저장
	imp_stus->chil_pid = (pid_t *)malloc(sizeof(pid_t) * num);
	// child process가 어떻게 종료됐는지 상태 저장하는 변수 배열 저장
	imp_stus->pipeFd = (int *)malloc(sizeof(int) * num);
	// pipFd[2]배열 저장
	imp_stus->pipeFd = (int **)malloc((sizeof(int *) * num);
	i = 0;
	while (i < num)
		imp_stus->pipeFd[i ++] = (int *)malloc(sizeof(int) * 2);
}
