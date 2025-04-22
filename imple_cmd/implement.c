/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   implement.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 16:57:12 by woonkim           #+#    #+#             */
/*   Updated: 2025/04/27 21:02:08 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void create_pipFd(t_cmd_info *t_cmd, t_imp_stus *imp_stus);
static void child_work(t_object *object, t_imp_stus *imp_stus);

// 명령어와 환경 병수 링크드 리스크로 받음
// t_cmd : 명령어에 대한 meta data를 저장하고 있는 node
// env : 환경 변수를 저장하고 있는 node
// 명령어가 없는 경우엔 cmd에 NULL (명령어가 없는 경우는 없음)
void implement(t_object *object)
{
	t_imp_stus imp_stus;
	pid_t pid;

	// imp_stus 초기화
	init_t_imp_stus(&imp_stus);
	// 명령어 갯수에 맞게 2차원 pipe 배열 생성, 저장은 imp_stus 구조체에
	create_pipFd(object->cmd_info, &imp_stus);
	// 부모 process부분, 명령어 반복 실행 (cur_c_n은 index로 쓰이기에 0부터 시작)
	while (imp_stus.cur_c_n < imp_stus.total_c_n)
	{
		// input이 있다면 열어서 fd반환, 없다면 -1
		imp_stus.input_fd = is_input(object);
		// builtins check후 맞다면 builtin 실행
		if (is_builtins(object->cmd_info))
		{
			execute_buitins(object);
			continue ;
		}
		// 실행파일 준비 및 실행
		prepare_and_fork(&imp_stus);
		// 자식 process 실행
		if (pid == 0)
			child_work(object, &imp_stus); // 실행 완료되면 pipe buffer에 값 저장됨
		// 부모 process 실행
		else 
		{
			object->cmd_info = object->cmd_info->next; // 다음 명령어로 이동
			// TODO : pipeFd 배열에 pipe가 10개 이상 쌓이면 해제해주기
		}
	}
	// TODO : pipe배열 모두 close및 free해주기
	// STDIN : 모니터로 변경하기 (만약 소실됐다면?)
}

// 파이프 생성 및 fork하는 함수
prepare_fork(int **pipeFd, int *pid)
{
	pipeFd[0] = (int *)malloc(sizeof(int) * 2);
	pipe(pipeFd[0]);
	*pid = fork();
}

// 자식 process를 명령어에 맞게 execve하여 동작하게 함
// 자식 process의 input과 output fd를 dup2로 적절히 컨트롤 해야함
static void child_work(t_object *object, t_imp_stus *imp_stus)
{
	char **envp;

	envp = NULL;
	// 읽기 fd close, 자식을 쓰기만 함
	close(imp_stus->pipeFd[0][0]);
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
	// TODO : 링크드리스크 -> 2차원 배열 환경변수 전환
	 if (execve(object->cmd_info->cmd_path, \
	 	object->cmd_info->evecve_argv, envp) == -1)
    	throw_error("FAILD EXECVE", object);
}

// t_cmd를 받아서 명령어 숫자를 세고 그에 맞게 pipeFd 배열 생성
// pipFd는 각 index마다 fd closs하고 index free하고 pipFd free해야 함
static void create_pipFd(t_cmd_info *t_cmd, t_imp_stus *imp_stus)
{
	int i;
	int num;
	char **pipFd;

	num = 0;
	while (t_cmd)
	{
		num ++;
		t_cmd = t_cmd->next;
	}
	// 총 명령어 갯수 저장
	imp_stus->total_c_n = num;
	// 맨 뒤에 NULL 넣기 위한 자리, char **아니어도 괜찮음
	pipFd = (int **)malloc((sizeof(int *) * num) + 1);
	i = 0;
	while (i < num)
		pipFd[i ++] = (int *)malloc(sizeof(int) * 2);
	pipFd[i] = NULL;
	imp_stus->pipeFd = pipFd;
}

