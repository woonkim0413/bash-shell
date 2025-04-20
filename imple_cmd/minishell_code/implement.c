/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   implement.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 16:57:12 by woonkim           #+#    #+#             */
/*   Updated: 2025/04/20 19:54:33 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// t_cmd : 명령어 저장된 링크드 리스크의 첫 노드
// env : 환경 변수를 저장하고 있는 링크드 리스크의 첫 노드

void implement(t_cmd_info* t_cmd, t_env *env)
{
	int pipeFd[2];
	int child_status;
	int t_pid;

	pipe(pipeFd);
	{
		// 다음 파이프라인이 존재하면 flag
	}
}

void ft_execve(t_cmd_info* t_cmd, t_env *env)
{

}
