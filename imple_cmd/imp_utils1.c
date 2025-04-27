/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   imp_utils1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 17:02:03 by woonkim           #+#    #+#             */
/*   Updated: 2025/04/27 15:37:52 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void redirect_process2(t_imp_stus *imp_stus, t_redirect *redirect);

void	init_t_imp_stus(t_imp_stus *imp_stus)
{
	imp_stus->i = 0;
	imp_stus->cur_c_n = 0;
	imp_stus->total_c_n = 0;
	imp_stus->input_fd = -1;
	imp_stus->output_fd = -1;
	imp_stus->pipeFd = NULL;
	imp_stus->chil_pid = NULL;
	// 1이 카리키는 buffer(출력 buffer)을 가리키는 새로운 fd생성 
	imp_stus->stdoutFd = dup(STDOUT_FILENO);
}

// "<" redirection 처리
// - ("cat", "-l", "<", "file2"); 이런건 구현부에서 처리 가능
// - ("<", "file1", "cat"); 이런 것도 처리 가능
// - ("cat", "-l" "<<EOF"); 이런건 parsing부에서 처리를 해줘야 함

// 이전 명령어 유무 check 
// - 이전 명령어가 있다면 파이프라인으로 input이 들어옴
// - output처리를 따로 안 해줘도 다음 명령어 실행시에 input으로써 처리됨
// - < file1.txt cat | < file2.txt wc -l | tee file5.txt 이런거 처리되고 있나?
void	input_output_setting(t_object *object, t_imp_stus *imp_stus)
{
	// 이전 명령어 파이프 output을 현재 명령어 input으로 연결
	if (object->cmd_info->prev != NULL)
	{
		dup2(imp_stus->pipeFd[imp_stus->cur_c_n - 1][0], STDIN_FILENO);
		close(imp_stus->pipeFd[imp_stus->cur_c_n - 1][0]);
	}
	// output을 pipe쓰기 fd로 rediection 
	// (자식 출력은 마지막 명령어는 부모 터미널 기본은 파이프 버퍼)
	if (object->cmd_info->next != NULL)
		dup2(imp_stus->pipeFd[imp_stus->cur_c_n][1], STDOUT_FILENO);
	close(imp_stus->pipeFd[imp_stus->cur_c_n][1]);
	redirect_process(object, imp_stus);
}

static void redirect_process(t_object *object, t_imp_stus *imp_stus)
{
	t_redirect *redirect;

	redirect = object->cmd_info->redirect;
	while (redirect)
	{
		redirect_process2(imp_stus, redirect);
		// >> redirection이 있는 경우
		if (redirect->type == TOKEN_APPEND)
		{
			imp_stus->output_fd = open(redirect->file_path, \
								O_WRONLY | O_CREAT | O_APPEND, 0644);
			dup2(imp_stus->output_fd, STDOUT_FILENO);
			close(imp_stus->output_fd);
		}
		// << redirection이 있는 경우
		if (redirect->type == TOKEN_HEREDOC)
		{
			dup2(redirect->heredoc_fd, STDIN_FILENO);
			close(redirect->heredoc_fd);
		}
		// 다음 redirect node로 이동
		redirect = redirect->next;
	}
}

static void redirect_process2(t_imp_stus *imp_stus, t_redirect *redirect)
{
	// < redirection이 있는 경우
		if (redirect->type == TOKEN_REDIR_IN)
		{
			imp_stus->input_fd = open(redirect->file_path, O_RDONLY);
			dup2(imp_stus->input_fd, STDIN_FILENO);
			close(imp_stus->input_fd);
		}
		// > redirection이 있는 경우
		if (redirect->type == TOKEN_REDIR_OUT)
		{
			imp_stus->output_fd = open(redirect->file_path, \
								O_WRONLY | O_CREAT | O_TRUNC, 0644);
			dup2(imp_stus->output_fd, STDOUT_FILENO);
			close(imp_stus->output_fd);
		}
}
