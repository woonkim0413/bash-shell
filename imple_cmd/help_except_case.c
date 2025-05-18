/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help_except_case.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 09:50:52 by woonkim           #+#    #+#             */
/*   Updated: 2025/05/17 16:02:43 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 싱글 쿼터나 더블 쿼터면 find_path에서 명령어를 찾지 못하고 에러를 출력하도록 더미값 넣어줌
void	single_duuble_quates_check(t_object *object)
{
	if (ft_strncmp(object->cmd_info->cmd, "", 1) == 0 && \
		ft_strncmp(object->cmd_info->evecve_argv[0], "", 1) == 0)
	{
		free(object->cmd_info->cmd);
		free(object->cmd_info->evecve_argv[0]);
		free(object->cmd_info->evecve_argv);
		object->cmd_info->cmd = ft_strdup("fucking");
		object->cmd_info->evecve_argv = (char **)malloc(sizeof(char *) * 2);
		object->cmd_info->evecve_argv[0] = ft_strdup("fucking");
		object->cmd_info->evecve_argv[1] = NULL;
	}
}
