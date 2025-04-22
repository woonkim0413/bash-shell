/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   implement.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 16:57:12 by woonkim           #+#    #+#             */
/*   Updated: 2025/04/27 21:00:26 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int **create_pipFd(t_cmd_info *t_cmd);
static void child_work(t_object *object, int* pipeFd);

// 명령어 구조체 링크드 리스크와 환경 병수 링크드 리스크 받음
// t_cmd : 현재 실행할 명령어
// env : 환경 변수를 저장하고 있는 링크드 리스크의 첫 노드
// 명령어가 없는 경우엔 cmd에 NULL
void implement(t_object *object)
{
	t_imp_stus imp_stus;
	int **pipeFd;
	pid_t pid;

	// 명령어 순서에 맞게 pipe 생성
	imp_stus.pipeFd = create_pipFd(object->cmd_info);
	// 부모 process부분, 명령어 반복 실행
	while (object->cmd_info != NULL) 
	{
		// input이 있다면 열어서 fd반환, 없다면 -1
		object->cmd_info->input_fd = is_input(object);
		// builtins check
		if (is_builtins(object->cmd_info))
			execute_buitins(object);
		// 실행파일 준비 및 실행
		prepare_and_fork(&imp_stus);
		// 자식 process 실행
		if (pid == 0)
			child_work(object, &imp_stus); // 실행 완료되면 pipe buffer에 값 저장됨
		object->cmd_info = object->cmd_info->next; // 다음 명령어로 이동
	}
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
static int **create_pipFd(t_cmd_info *t_cmd)
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
	num ++; // 맨 뒤에 NULL, char **아니어도 괜찮음
	pipFd = (int **)malloc(sizeof(int *) * num);
	i = 0;
	while (i < num - 1)
		pipFd[i ++] = (int *)malloc(sizeof(int) * 2);
	pipFd[i] = NULL;
	return (pipFd);
}

