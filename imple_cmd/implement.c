/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   implement.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 16:57:12 by woonkim           #+#    #+#             */
/*   Updated: 2025/04/21 19:29:54 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int **create_pipFd(t_cmd_info *t_cmd);
static void parent_work(t_cmd_info *t_cmd, int *pipeFd);
static void child_work(t_cmd_info* t_cmd, t_env* env, int* pipeFd);

// 명령어 구조체 링크드 리스크와 환경 병수 링크드 리스크 받음
// t_cmd : 현재 실행할 명령어
// env : 환경 변수를 저장하고 있는 링크드 리스크의 첫 노드
// 명령어가 없는 경우엔 cmd에 NULL
void implement(t_cmd_info* t_cmd, t_env* env)
{
	int **pipeFd;
	int child_status;
	pid_t pid;

	pipeFd = create_pipeFd(t_cmd);
	if (pipeFd[0] != NULL)
		pipe(pipeFd);
	pid = fork();
	if (pid == 0)
		child_work(t_cmd, env, pipeFd);
	// 부모 process, 명령어 반복 실행
	whild () 
	{
		parent_work(t_cmd, pipeFd);	
	}
}

// 자식 process를 명령어에 맞게 execve하여 동작하게 함
// 자식 process의 input과 output fd를 dup2로 적절히 컨트롤 해야함
static void child_work(t_cmd_info* t_cmd, t_env* env, int* pipeFd)
{
	char **envp;

	envp = NULL;
	// 읽기 fd close, 자식을 쓰기만 함
	close(pipeFd[0]);
	// inputFd가 있다면 input을 stdin으로 받도록 설정
	if (t_cmd->input_fd != -1)
	{
		dup2(t_cmd->input_fd, 0);
		close(t_cmd->input_fd);
	}
	// output은 pipeFd를 통해 부모 process에 전달
	dup2(pipeFd[1], 1);
	close(pipeFd[1]);
	// find_path로 찾은 path는 t_cmd의 cmd_path에 저장
    find_path(t_cmd, env);
	// 명령어가 없는 경우 free
	if (t_cmd->cmd_path == NULL)
		throw_error("NOT FOUND CMD", t_cmd);
	// TODO : 링크드리스크 -> 2차원 배열 환경변수 전환
	 if (execve(t_cmd->cmd_path, t_cmd->evecve_argv, envp) == -1)
    	throw_error("FAILD EXECVE", t_cmd);
}

// 부모 process 동작 정의
static void parent_work(t_cmd_info *t_cmd, int *pipeFd)
{
	// 쓰기 fd close, 부모는 읽기만 함
	close(pipeFd[1]);
	if (t_cmd->next == NULL);	
	
}

// t_cmd를 받아서 명령어 숫자를 세고 그에 맞게 pipeFd 배열 생성
// pipFd는 각 index마다 fd closs하고 index free하고 pipFd free해야 함
// 만약 첫 번째 노드의 t_cmd->cmd가 NULL이면 실행명령 없음
static int **create_pipFd(t_cmd_info *t_cmd)
{
	int i;
	int num;
	char **pipFd;

	num = 0;
	while (t_cmd && t_cmd->cmd)
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

