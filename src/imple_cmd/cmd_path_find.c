/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path_find.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 22:13:19 by woonkim           #+#    #+#             */
/*   Updated: 2025/05/20 14:31:16 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	memory_free(char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
		free(argv[i++]);
	free(argv);
}

static char	**make_path(char **paths, char *command)
{
	int		i;
	char	*temp;

	i = -1;
	while (paths[++i])
	{
		temp = ft_strjoin(paths[i], "/");
		free(paths[i]);
		paths[i] = temp;
		temp = ft_strjoin(paths[i], command);
		free(paths[i]);
		paths[i] = temp;
	}
	return (paths);
}

static char	*check_and_return_path(t_cmd_info *t_cmd, char **paths)
{
	int		i;
	char	*cmd_path;

	i = 0;
	while (paths[i])
	{
		if (access(paths[i], X_OK) == 0)
		{
			if (!ft_strncmp(paths[i], "/mnt/c/WINDOWS/temp", \
				ft_strlen(paths[i])))
				break ;
			cmd_path = ft_strdup(paths[i]);
			i = 0;
			memory_free(paths);
			return (cmd_path);
		}
		i++;
	}
	if (!t_cmd->redirect)
		write(2, "This command does not exist\n", 29);
	memory_free(paths);
	return (NULL);
}

static int	is_executable_file(const char *path)
{
	struct stat	st;

	if (stat(path, &st) < 0)
		return (0);
	if (!S_ISREG(st.st_mode))
		return (0);
	if (access(path, X_OK) < 0)
		return (0);
	return (1);
}

int	find_path(t_cmd_info *t_cmd, t_env *env)
{
	t_env	*temp;
	char	**paths;

	temp = env;
	if (t_cmd->cmd && is_executable_file(t_cmd->cmd))
	{
		t_cmd->cmd_path = t_cmd->cmd;
		return (1);
	}
	while (temp)
	{
		if (ft_strncmp(temp->key, "PATH", 4) == 0)
		{
			paths = ft_split(temp->value, ':');
			paths = make_path(paths, t_cmd->cmd);
			t_cmd->cmd_path = check_and_return_path(t_cmd, paths);
			if (t_cmd->cmd_path == NULL)
				return (0);
			return (1);
		}
		temp = temp->next;
	}
	t_cmd->cmd_path = NULL;
	return (0);
}
