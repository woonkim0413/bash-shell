/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   imp_utils1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:19:05 by woonkim           #+#    #+#             */
/*   Updated: 2025/05/13 19:26:40 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **env_to_char(t_env *env)
{
	int env_num;
	int i;
	char *str_temp;
	char **str;
	t_env *temp;

	temp = env;
	env_num = 0;
	while (temp)
	{
		temp = temp->next;
		env_num++;
	}
	str = (char **)malloc(sizeof(char *) * (env_num + 1));
	str[env_num] = NULL;
	i = 0;
	while (i < env_num)
	{
		str_temp = ft_strjoin(env->key, "=");
		str[i++] = ft_strjoin(str_temp, env->value);
		env = env->next;
		free(str_temp);
	}
	return (str);
}

static int argv_end_check(char *argv)
{
	int flag;

	flag = 1;
	if (argv == NULL)
		return (0);
	if (!ft_strncmp(argv, "<", ft_strlen(argv)))
		flag = 0;
	if (!ft_strncmp(argv, "<<", ft_strlen(argv)))
		flag = 0;
	if (!ft_strncmp(argv, ">", ft_strlen(argv)))
		flag = 0;
	if (!ft_strncmp(argv, ">>", ft_strlen(argv)))
		flag = 0;
	return (flag);
}

// 락윤이가 건네주는 execve는 한 파이프 안에 있는 모든 단어들임
// 이것을 execve에 건네줄 수 있는 구조로 바꿔야 함
void create_execve_args(t_cmd_info *cmd_info)
{
	char **argv;
	int i;
	int j;
	int k;

	i = -1;
	argv = cmd_info->evecve_argv;
	while (argv[++i])
	{
		if (!ft_strncmp(cmd_info->cmd, argv[i], ft_strlen(cmd_info->cmd)))
		{
			j = i;
			while (argv_end_check(argv[j]))
				j++;
			cmd_info->evecve_argv = (char **)malloc(sizeof(char *) *
													(j - i + 1));
			k = 0;
			while (i < j)
				cmd_info->evecve_argv[k++] = ft_strdup(argv[i++]);
			cmd_info->evecve_argv[k] = NULL;
			free_string_arr(&argv);
			// free_doublechar(argv);
			return;
		}
	}
}

void free_doublechar(char **argv)
{
	int i;

	i = 0;
	while (argv[i])
		free(argv[i++]);
	free(argv);
}

int cmd_null_check(t_object *object,t_imp_stus *imp_stus)
{
	(void)imp_stus;
	if (object->cmd_info->cmd)
		return (0);
	return (1);
}
