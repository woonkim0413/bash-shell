/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   imp_utils1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 17:02:03 by woonkim           #+#    #+#             */
/*   Updated: 2025/04/23 21:12:00 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_t_imp_stus(t_imp_stus *imp_stus)
{
	imp_stus->i = 0;
	imp_stus->cur_c_n = 0;
	imp_stus->total_c_n = 0;
	imp_stus->input_fd = -1;
	imp_stus->output_fd = -1;
	imp_stus->pipeFd = NULL;
	imp_stus->chil_pid = NULL;
	imp_stus->chil_e_stus = NULL;
	// 1이 카리키는 buffer(출력 buffer)을 가리키는 새로운 fd생성 
	imp_stus->stdoutFd = dup(STDOUT_FILENO);
}

// "<" redirection 처리
// - ("cat", "-l", "<", "file2"); 이런건 구현부에서 처리 가능
// - ("cat", "-l" "<<", EOF); 이런건 parsing부에서 처리를 해줘야 함

// 이전 명령어 유무 check 
// - 이전 명령어가 있다면 파이프라인으로 input이 들어옴
// - output처리를 따로 안 해줘도 다음 명령어 실행시에 input으로써 처리됨

// 만약 마지막에 builtins가 호출되어 stdout을 호출해야 한다면?
// - int saved_stdout = dup(STDOUT_FILENO);로 미리 imp_stus->stdoutFd에 stdout Fd저장
void	input_check(t_object *object)
{
	
}