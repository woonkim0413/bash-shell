/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 15:07:00 by woonkim           #+#    #+#             */
/*   Updated: 2025/05/19 15:14:22 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	update_oldpwd(t_object *object, t_imp_stus *imp_stus);
static void	update_pwd(t_object *object, t_imp_stus *imp_stus);


void handle_flag(t_imp_stus *imp_stus, t_object *object, int flag, char *path)
{
	if (flag == -1)
	{
		write(imp_stus->stdoutFd, "Error: cd: ", 10);
		write(imp_stus->stdoutFd, path, ft_strlen(path));
		write(imp_stus->stdoutFd, ": No Such file or directory\n", 29);
		object->last_exit_status = 1;
	}
	else
	{
		update_oldpwd(object, imp_stus);
		update_pwd(object, imp_stus);
	}
}

static void	update_oldpwd(t_object *object, t_imp_stus *imp_stus)
{
	t_env	*temp;
	char	*export_arg;

	temp = object->env;
	while (temp)
	{
		if (!ft_strncmp(temp->key, "PWD", ft_strlen(temp->key)))
		{
			export_arg = ft_strjoin("OLDPWD=", ft_strdup(temp->value));
			free(object->cmd_info->evecve_argv[1]);
			object->cmd_info->evecve_argv[1] = export_arg;
			execute_export(object, imp_stus);
			return ;
		}
		temp = temp->next;
	}
}

static void	update_pwd(t_object *object, t_imp_stus *imp_stus)
{
	char	*export_arg;
	char	buffer[4096];

	getcwd(buffer, sizeof(buffer) - 1);
	export_arg = ft_strjoin("PWD=", buffer);
	free(object->cmd_info->evecve_argv[1]);
	object->cmd_info->evecve_argv[1] = export_arg;
	execute_export(object, imp_stus);
}

