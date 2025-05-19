/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 13:50:45 by woonkim           #+#    #+#             */
/*   Updated: 2025/05/19 10:51:27 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	change_wave(char **path, t_object *object);
static void	change_dash(char **path, t_object *object);
static void	update_oldpwd(t_object *object, t_imp_stus *imp_stus);
static void	update_pwd(t_object *object, t_imp_stus *imp_stus);

// PWD 위치를 변경해주는 명령어다 
int	execute_cd(t_object *object, t_imp_stus *imp_stus)
{
	int		flag;
	char	*path;

	path = ft_strdup(object->cmd_info->evecve_argv[1]);
	if (path == NULL)
		return (1);
	change_wave(&path, object);
	change_dash(&path, object);
	flag = chdir(path);
	if (flag == -1)
	{
		write(imp_stus->stdoutFd, "Error: cd: ", 10);
		write(imp_stus->stdoutFd, path, ft_strlen(path));
		write(imp_stus->stdoutFd,": No Such file or directory\n", 29);
		object->last_exit_status = 1;
	}
	else
	{
		update_oldpwd(object, imp_stus);
		update_pwd(object, imp_stus);
	}
	free(path);
	return (1);
}

static void	change_wave(char **path, t_object *object)
{
	t_env	*temp;

	if (!ft_strncmp(*path, "~", ft_strlen(*path)))
	{
		temp = object->env;
		while (temp)
		{
			if (!ft_strncmp(temp->key, "HOME", ft_strlen(temp->key)))
			{
				free(*path);
				*path = ft_strdup(temp->value);
				return ;
			}
			temp = temp->next;
		}
	}
}

static void	change_dash(char **path, t_object *object)
{
	t_env	*temp;

	if (!ft_strncmp(*path, "-", ft_strlen(*path)))
	{
		temp = object->env;
		while (temp)
		{
			if (!ft_strncmp(temp->key, "OLDPWD", ft_strlen(temp->key)))
			{
				free(*path);
				*path = ft_strdup(temp->value);
				return ;
			}
			temp = temp->next;
		}
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

