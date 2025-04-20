/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   implement.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 16:57:12 by woonkim           #+#    #+#             */
/*   Updated: 2025/04/20 22:13:21 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <fcntl.h> // 추가 필요 (락윤이에게 건네주기)
#include <sys/wait.h> // 추가 필요 (락윤이에게 건네주기)

// t_cmd : 현재 실행할 명령어
// env : 환경 변수를 저장하고 있는 링크드 리스크의 첫 노드
void implement(t_cmd_info* t_cmd, t_env *env)
{
	int pipeFd[2];
	int child_status;
	pid_t pid;

	pipe(pipeFd);
	pid = fork();
	// 자식 process
	if (pid == 0)
		child_work(t_cmd, env, pipeFd);
	// 부모 process
	// 쓰기 fd close, 부모는 일기만 함
	close(pipeFd[1]);
}

void child_work(t_cmd_info* t_cmd, t_env *env, int *pipeFd)
{
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
}		
