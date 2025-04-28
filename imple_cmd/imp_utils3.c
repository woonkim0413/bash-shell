/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   imp_utils3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 14:38:18 by woonkim           #+#    #+#             */
/*   Updated: 2025/04/28 18:18:00 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int one_builtin_case(t_object *object, t_imp_stus *imp_stus)
{
	char 	*cmd;
	int		flag;

	flag = 0;
	if (object->cmd_info->prev == NULL && object->cmd_info->next == NULL)
	{
		cmd = object->cmd_info->cmd;
		if (!ft_strncmp("echo", cmd, ft_strlen(cmd)))
			flag = execute_echo(object, imp_stus);
		else if (!ft_strncmp("pwd", cmd, ft_strlen(cmd)))
			flag = execute_pwd(object, imp_stus);
		else if (!ft_strncmp("unset", cmd, ft_strlen(cmd)))
			flag = execute_unset(object, imp_stus);
		else if (!ft_strncmp("export", cmd, ft_strlen(cmd)))
			flag = execute_export(object, imp_stus);
		else if (!ft_strncmp("env", cmd, ft_strlen(cmd)))
			flag = execute_env(object, imp_stus);
		else if (!ft_strncmp("exit", cmd, ft_strlen(cmd)))
			flag = execute_exit(object, imp_stus);
	}
	return (flag);
}
