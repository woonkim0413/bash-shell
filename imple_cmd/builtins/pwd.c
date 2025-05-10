/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 15:37:59 by woonkim           #+#    #+#             */
/*   Updated: 2025/05/05 23:31:47 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char *find_cur_name(struct stat *current);

int execute_pwd(t_object *object, t_imp_stus *imp_stus, int prev_cwd_fd)
{
	struct stat 	current_stat;
	struct stat 	parent_stat;
	char			*cur_name;
	int				cwd_fd;

	stat(".", &current_stat);
	stat("..", &parent_stat);
	if (parent_stat.st_ino == current_stat.st_ino && \
		parent_stat.st_dev == current_stat.st_dev)
	{
		return (1);
	}
	// 현재 디렉토리 이름 찾기
	cur_name = find_cur_name(&current_stat);
	cwd_fd = open(".", O_RDONLY);
	// 상위 디렉토리로 이동
	chdir("..");
	// 재귀 호출
	execute_pwd(object, imp_stus, cwd_fd);
	// 디렉토리 출력
	write(STDOUT_FILENO, "/", 1);
	write(STDOUT_FILENO, cur_name, ft_strlen(cur_name));
	free(cur_name);
	// 다시 하위 디렉토리로 이동 (최종적으론 원래 위치로 back)
	if (prev_cwd_fd != -1)
		fchdir(prev_cwd_fd);
	else
		write(STDOUT_FILENO, "\n", 1);
	return (1);
}

// 현재 directory 정보 찾아서 반환
static char *find_cur_name(struct stat *current)
{
	struct dirent	*entry;
	DIR 			*dir;
	char			*cur_name;

	dir = opendir("..");
	while (1)
	{
		entry = readdir(dir);
		if (entry->d_ino == current->st_ino)
		{
			cur_name = strdup(entry->d_name);
			break ;
		}
	}
	closedir(dir);
	return (cur_name);
}
