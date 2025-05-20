/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 13:50:45 by woonkim           #+#    #+#             */
/*   Updated: 2025/05/20 10:56:37 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	change_wave(char **path, t_object *object);
static void	change_dash(char **path, t_object *object);

// PWD 위치를 변경해주는 명령어다 
int	execute_cd(t_object *object, t_imp_stus *imp_stus)
{
	int		flag;
	char	*path;
	char	**argv;

	argv = object->cmd_info->evecve_argv;
	if (argv[1] && argv[2])
	{
		write(imp_stus->stdout_fd, "Error: cd: ", 10);
		write(imp_stus->stdout_fd,": Too many arguments\n", 22);
		object->last_exit_status = 1;
		return (1);
	}
	path = ft_strdup(argv[1]);
	if (path == NULL)
		return (1);
	change_wave(&path, object);
	change_dash(&path, object);
	flag = chdir(path);
	handle_flag(imp_stus, object, flag, path);
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