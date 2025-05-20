/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   imp_utils1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:19:05 by woonkim           #+#    #+#             */
/*   Updated: 2025/05/20 15:50:07 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_size(t_cmd_info *cmd_info);
static int	argv_redirection_check(char *argv, t_cmd_info *cmd_info);

char	**env_to_char(t_env *env, int i)
{
	int		env_num;
	char	*str_temp;
	char	**str;
	t_env	*temp;

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

static	void	set_argv(t_cmd_info *cmd_info, char **argv, int i)
{
	int		j;

	j = 0;
	while (cmd_info->evecve_argv[i])
	{
		if (argv_redirection_check(cmd_info->evecve_argv[i], cmd_info))
			i = i + 2;
		else
		{
			argv[j++] = ft_strdup(cmd_info->evecve_argv[i++]);
		}
	}
	free_doublechar(cmd_info->evecve_argv);
	cmd_info->evecve_argv = argv;
}

void	create_execve_args(t_cmd_info *cmd_info)
{
	char	**argv;
	int		total_size;
	int		i;

	total_size = count_size(cmd_info);
	if (!(cmd_info->evecve_argv[0]))
		return ;
	total_size = count_size(cmd_info);
	argv = ft_calloc(sizeof(char *), total_size);
	i = -1;
	while (cmd_info->evecve_argv[++i])
	{
		if (cmd_info->cmd && !ft_strncmp(cmd_info->cmd, \
		cmd_info->evecve_argv[i], ft_strlen(cmd_info->cmd)) && \
		ft_strlen(cmd_info->cmd) == ft_strlen(cmd_info->evecve_argv[i]))
		{
			set_argv(cmd_info, argv, i);
			return ;
		}
	}
	free_string_arr(&argv);
}

static int	count_size(t_cmd_info *cmd_info)
{
	int	i;
	int	total_size;

	i = 0;
	total_size = 0;
	while (cmd_info->evecve_argv[i])
	{
		if (argv_redirection_check(cmd_info->evecve_argv[i], cmd_info))
		{
			i = i + 2;
		}
		else
		{
			i ++;
			total_size ++;
		}
	}
	total_size ++;
	return (total_size);
}

static int	argv_redirection_check(char *argv, t_cmd_info *cmd_info)
{
	int	flag;

	flag = 0;
	if (!ft_strncmp(argv, "<", ft_strlen(argv)) && cmd_info->redirect)
		flag = 1;
	else if (!ft_strncmp(argv, "<<", ft_strlen(argv)) && cmd_info->redirect)
		flag = 1;
	else if (!ft_strncmp(argv, ">", ft_strlen(argv)) && cmd_info->redirect)
		flag = 1;
	else if (!ft_strncmp(argv, ">>", ft_strlen(argv)) && cmd_info->redirect)
		flag = 1;
	return (flag);
}
