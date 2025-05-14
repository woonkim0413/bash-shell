/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help_except_case.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 09:50:52 by woonkim           #+#    #+#             */
/*   Updated: 2025/05/14 09:52:55 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 싱글 쿼터나 더블 쿼터면 find_path에서 명령어를 찾지 못하고 에러를 출력하도록 더미값 넣어줌
void	single_duuble_quates_check(t_object *object)
{
	if ((object->cmd_info->cmd && object->cmd_info->cmd[0] == '\0') && \
		(object->cmd_info->evecve_argv && object->cmd_info->evecve_argv[0] == NULL))
	{
		free(object->cmd_info->cmd);
		free(object->cmd_info->evecve_argv);
		object->cmd_info->cmd = ft_strdup("fucking");
		object->cmd_info->evecve_argv = (char **)malloc(sizeof(char *) * 2);
		object->cmd_info->evecve_argv[0] = ft_strdup("fucking");
		object->cmd_info->evecve_argv[1] = NULL;
	}
}