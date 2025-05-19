/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 12:30:20 by woonkim           #+#    #+#             */
/*   Updated: 2025/05/19 10:56:50 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_env(t_object *object, t_imp_stus *imp_stus)
{
	t_env	*temp;
	char	*str;	

	(void)imp_stus;
	str = object->cmd_info->evecve_argv[1];
	if (str)
	{
		write(imp_stus->stdoutFd, "Error: '", 6);
		write(imp_stus->stdoutFd, str, ft_strlen(str));
		write(imp_stus->stdoutFd, "': No such file or directory\n", 29);
		object->last_exit_status = 127;
		return (1);
	}
	temp = object->env;
	while (temp)
	{
		if (temp->key && temp->value)
			printf("%s=%s\n", temp->key, temp->value);
		temp = temp->next;
	}
	return (1);
}
