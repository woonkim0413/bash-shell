/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_setting.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 17:02:03 by woonkim           #+#    #+#             */
/*   Updated: 2025/05/14 11:34:50 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void redirect_process2(t_imp_stus *imp_stus, t_redirect *redirect);
static void redirect_process(t_object *object, t_imp_stus *imp_stus);

// 이전 명령어 유무 check 
// - 이전 명령어가 있다면 파이프라인으로 input이 들어옴
// - output처리를 따로 안 해줘도 다음 명령어 실행시에 input으로써 처리됨
// - < file1.txt cat | < file2.txt wc -l | tee file5.txt 이런거 처리되고 있나?
void	input_output_setting(t_object *object, t_imp_stus *imp_stus, \
		int one_builtin_flag)
{
	// stderr 처리
	if (!one_builtin_flag)
	{
		close(imp_stus->stderr_pipe[0]);
		dup2(imp_stus->stderr_pipe[1], STDERR_FILENO);
		close(imp_stus->stderr_pipe[1]);
		// child에선 현재 파이프 buffer 읽기 fd를 닫음
		close(imp_stus->pipeFd[imp_stus->cur_c_n][0]);
		// 이전 명령어 파이프 input을 현재 명령어 input으로 연결
		if (object->cmd_info->prev != NULL)
		{
			dup2(imp_stus->pipeFd[imp_stus->cur_c_n - 1][0], STDIN_FILENO);
			close(imp_stus->pipeFd[imp_stus->cur_c_n - 1][0]);
		}
		// 현재 명령어 파이프 쓰기 fd가 가리키는 buffer을 stdout도 가리키게 함 
		// (마지막 명령어 출력은 redirection없이 부모 stdout 사용)
		if (object->cmd_info->next != NULL)
			dup2(imp_stus->pipeFd[imp_stus->cur_c_n][1], STDOUT_FILENO);
		close(imp_stus->pipeFd[imp_stus->cur_c_n][1]);
	}
	redirect_process(object, imp_stus);
}

static void redirect_process(t_object *object, t_imp_stus *imp_stus)
{
	t_redirect *redirect;

	redirect = object->cmd_info->redirect;
	while (redirect)
	{
		// 히어독이 아니면서 리다이렉션 대상 파일이 없는 경우
		if (redirect->type != TOKEN_HEREDOC && \
			access(redirect->file_path, F_OK) != 0)
		{
			write(2, redirect->file_path, ft_strlen(redirect->file_path));
			write(2, ": No such file or directitory\n", 31);
			free_stus_and_object(object, imp_stus);
			exit(1);	
		}
		redirect_process2(imp_stus, redirect);
		// >> redirection이 있는 경우
		if (redirect->type == TOKEN_APPEND)
		{
			imp_stus->output_fd = open(redirect->file_path, \
								O_WRONLY | O_CREAT | O_APPEND, 0644);
			dup2(imp_stus->output_fd, STDOUT_FILENO);
			close(imp_stus->output_fd);
		}
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
	else if (redirect->type == TOKEN_REDIR_OUT)
	{
		imp_stus->output_fd = open(redirect->file_path,
								   O_WRONLY | O_CREAT | O_TRUNC, 0644);
		dup2(imp_stus->output_fd, STDOUT_FILENO);
		close(imp_stus->output_fd);
	}
	else if (redirect->type == TOKEN_HEREDOC)
	{
		imp_stus->input_fd = redirect->heredoc_fd;
		dup2(imp_stus->input_fd, STDIN_FILENO);
		close(imp_stus->input_fd);
	}
}
