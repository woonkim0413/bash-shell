/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   imp_utils1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:19:05 by woonkim           #+#    #+#             */
/*   Updated: 2025/05/19 13:04:52 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_option(char **evecve_argv, int index);
char **create_new_argv(t_cmd_info *cmd_info, int i, int j, int option_index);

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

static int argv_end_check(char *argv, t_cmd_info *cmd_info)
{
	int flag;

	flag = 1;
	if (argv == NULL)
		return (0);
	if (!ft_strncmp(argv, "<", ft_strlen(argv)) && cmd_info->redirect)
		flag = 0;
	if (!ft_strncmp(argv, "<<", ft_strlen(argv)) && cmd_info->redirect)
		flag = 0;
	if (!ft_strncmp(argv, ">", ft_strlen(argv)) && cmd_info->redirect)
		flag = 0;
	if (!ft_strncmp(argv, ">>", ft_strlen(argv)) && cmd_info->redirect)
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
	int option_index;

	i = -1;
	while (cmd_info->evecve_argv[++i])
	{
		if (cmd_info->cmd && !ft_strncmp(cmd_info->cmd, \
			cmd_info->evecve_argv[i], ft_strlen(cmd_info->cmd)))
		{
			j = i;
			while (argv_end_check(cmd_info->evecve_argv[j], cmd_info))
				j++;
			// J 다음에 evecve_argv에 -옵션이 있는지 확인함 있다면 해당 index반환, 없다면 0
			option_index = check_option(cmd_info->evecve_argv, j);
			argv = create_new_argv(cmd_info, i, j, option_index);
			free_doublechar(cmd_info->evecve_argv);
			cmd_info->evecve_argv = argv;
			return;
		}
	}
}

char **create_new_argv(t_cmd_info *cmd_info, int i, int j, int option_index)
{
	int		k;
	int 	end_index;
	char 	**argv;

	k = 0;
	end_index = j;
	// 플레그 넣을 공간 확장
	if (option_index)
		j++;
	argv = (char **)malloc(sizeof(char *) * (j - i + 1));
	while (i < end_index)
		argv[k++] = ft_strdup(cmd_info->evecve_argv[i++]);
	if (option_index)
		argv[k++] = ft_strdup(cmd_info->evecve_argv[option_index]);
	argv[k] = NULL;
	return (argv);
}

int	check_option(char **evecve_argv, int index)
{
	if (evecve_argv[index] == NULL)
		return (0);
	while (evecve_argv[index]) 
	{
		if (evecve_argv[index][0] == '-')
			return (index);
		index ++;
	}
	return (0);
}
