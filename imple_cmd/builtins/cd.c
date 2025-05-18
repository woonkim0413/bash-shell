/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 13:50:45 by woonkim           #+#    #+#             */
/*   Updated: 2025/05/17 15:31:33 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void change_wave_a_dash(char **path, t_object *object);
static void update_oldpwd(t_object *object, t_imp_stus *imp_stus);
static void update_pwd(t_object *object, t_imp_stus *imp_stus);

// PWD 위치를 변경해주는 명령어다 
int execute_cd(t_object *object, t_imp_stus *imp_stus)
{
	int		flag;
	char 	*path;

	path = ft_strdup(object->cmd_info->evecve_argv[1]);
	if (path == NULL)
		return (1);
	// path가 -나 ~인 경우 전환해주는 함수
	change_wave_a_dash(&path, object);
	flag = chdir(path);
	if (flag == -1)
	{
		write(imp_stus->stdoutFd, "bash: cd: ", 10);
		write(imp_stus->stdoutFd, path, ft_strlen(path));
		write(imp_stus->stdoutFd,": No Such file or directory\n", 29);
	}
	else
	{
		update_oldpwd(object, imp_stus);
		update_pwd(object, imp_stus);
	}
	free(path);
	return (1);
}

static void change_wave_a_dash(char **path, t_object *object)
{
	t_env *temp;

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
	else if (!ft_strncmp(*path, "~", ft_strlen(*path)))
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

static void update_oldpwd(t_object *object, t_imp_stus *imp_stus)
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

// 위치가 정상적으로 변경된 경우 PWD 환경변수 값을 변경하는 함수
// getcwd로 현재 위치 획득 -> 환경변수 arg값 생성 -> export 실행
// (export는 내부적으로 이미 환경 변수가 있다면 value를 update함)
static void update_pwd(t_object *object, t_imp_stus *imp_stus)
{
	char *export_arg;
	char buffer[4096];
	
	getcwd(buffer, sizeof(buffer) - 1);
	export_arg = ft_strjoin("PWD=", buffer);
	free(object->cmd_info->evecve_argv[1]); 
	// -> update_oldpwd가 실행하는 export에서 다 프리해줌
	object->cmd_info->evecve_argv[1] = export_arg;
	execute_export(object, imp_stus);
}

