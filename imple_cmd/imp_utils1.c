/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   imp_utils1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:19:05 by woonkim           #+#    #+#             */
/*   Updated: 2025/05/16 23:31:53 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **env_to_char(t_env *env, int i)
{
	int env_num;
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
// 단어 더 파싱할 수 있는지 고민해보기 ls > file1.txt -la
void create_execve_args(t_cmd_info *cmd_info)
{
	char **argv;
	int i;
	int j;
	int k;

	i = -1;
	while (cmd_info->evecve_argv[++i])
	{
		if (!ft_strncmp(cmd_info->cmd, cmd_info->evecve_argv[i], \
			ft_strlen(cmd_info->cmd)))
		{
			j = i;
			while (argv_end_check(cmd_info->evecve_argv[j]))
				j++;
			argv = (char **)malloc(sizeof(char *) * (j - i + 1));
			k = 0;
			while (i < j)
				argv[k++] = ft_strdup(cmd_info->evecve_argv[i++]);
			argv[k] = NULL;
			free_doublechar(cmd_info->evecve_argv);
			cmd_info->evecve_argv = argv;
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
