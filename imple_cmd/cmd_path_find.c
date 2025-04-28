/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path_find.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 22:13:19 by woonkim           #+#    #+#             */
/*   Updated: 2025/04/28 16:29:57 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void memory_free(char** argv)
{
	int	i;

	i = 0;
	while(argv[i])
		free(argv[i++]);
	free(argv);
}

static char **make_path(char **paths, char *command)
{
	int i;
	char *temp;

	i = -1;
	while (paths[++i])
	{
		// path + command를 합침 (join의 반환값은 malloc)
		// split의 반환값은 free해줬음
		// left, right에 둘 다 같은 변수를 쓰면 ++이 안 써짐
		temp = ft_strjoin(paths[i], "/");
		// split으로 생성된 malloc을 free함
		free(paths[i]);
		paths[i] = temp;
		temp = ft_strjoin(paths[i], command);
		// 첫 번째 join으로 생성된 malloc을 free함
		free(paths[i]);
		// 두 번째 join으로 완성된 "절대 주소 + / + 명령어" path
		paths[i] = temp;
	}
	return (paths);
}

static char *check_and_return_path(char **paths)
{
	int	i;
	char *cmd_path;

	i = 0;
	while (paths[i]) // 4번 반복
	{
		// 해당 위치에 명령어가 존재하는지 확인
		if (access(paths[i], F_OK) == 0)
		{
			cmd_path = ft_strdup(paths[i]);
			i = 0;
			while (paths[i])
				free(paths[i++]);
			free(paths);
			return (cmd_path);
		}
		i++;
	}
	write(2, "This command does not exist\n", 29);
	memory_free(paths);
	return (NULL);
}

void find_path(t_cmd_info* t_cmd, t_env* env)
{
	t_env *temp;
	char **paths;

	temp = env;
	// env의 몇 번째 노드에 PATH 환경변수가 들어 있는지 확인
	while (temp)
	{
		if (ft_strncmp(temp->key, "PATH", 4) == 0)
		{
			// 하나의 string에 담겨져 있는 path들을 :을 기준으로 쪼갬
			// 다 malloc으로 생성하기에 free해줘야 함
			paths = ft_split(temp->value, ':');
			paths = make_path(paths, t_cmd->cmd);
			t_cmd->cmd_path = check_and_return_path(paths);
			return ;
		}
		temp = temp->next;
	}
	// 명령어가 존재하지 않으면 null담음
	t_cmd->cmd_path = NULL;
}
