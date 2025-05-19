/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   imple_setting.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 15:55:16 by woonkim           #+#    #+#             */
/*   Updated: 2025/05/19 10:27:08 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// t_cmd를 받아서 명령어 숫자를 세고 그에 맞게 pipeFd 배열 생성
// pipFd는 각 index마다 fd closs하고 index free하고 pipFd free해야 함
void	setting_pipline(t_cmd_info *t_cmd, t_imp_stus *imp_stus)
{
	int	i;
	int	num;

	init_t_imp_stus(imp_stus);
	imp_stus->existing_cmd_info = t_cmd;
	num = 0;
	imp_stus->stdoutFd = dup(STDOUT_FILENO);
	imp_stus->stdinFd = dup(STDIN_FILENO);
	pipe(imp_stus->stderr_pipe);
	while (t_cmd)
	{
		num++;
		t_cmd = t_cmd->next;
	}
	imp_stus->total_c_n = num;
	imp_stus->chil_pid = (pid_t *)malloc(sizeof(pid_t) * num);
	imp_stus->pipeFd = (int **)malloc(sizeof(int *) * num);
	i = 0;
	while (i < num)
		imp_stus->pipeFd[i ++] = (int *)malloc(sizeof(int) * 2);
}

// 파이프 생성 및 fork()하는 함수
void	pipe_and_fork(t_imp_stus *imp_stus)
{
	pipe(imp_stus->pipeFd[imp_stus->cur_c_n]);
	imp_stus->chil_pid[imp_stus->cur_c_n] = fork();
}

void	init_t_imp_stus(t_imp_stus *imp_stus)
{
	imp_stus->i = 0;
	imp_stus->all_path = 0;
	imp_stus->cur_c_n = 0;
	imp_stus->total_c_n = 0;
	imp_stus->input_fd = -1;
	imp_stus->output_fd = -1;
	imp_stus->existing_cmd_info = NULL;
	imp_stus->pipeFd = NULL;
	imp_stus->chil_pid = NULL;
	imp_stus->stdoutFd = 0;
}
