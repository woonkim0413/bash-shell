/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent_wait_to_child.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 20:07:27 by woonkim           #+#    #+#             */
/*   Updated: 2025/05/19 14:56:09 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	error_process(t_object *object, t_imp_stus *imp_stus, pid_t ret);

void	wait_childs_process(t_object *object, t_imp_stus *imp_stus)
{
	pid_t	ret;
	int		raw_exit_stus;

	raw_exit_stus = 0;
	while (imp_stus->i < imp_stus->total_c_n)
	{
		ret = waitpid(-1, &raw_exit_stus, 0);
		error_process(object, imp_stus, ret);
		if (ret < 0)
			perror("waitpid error : ");
		if (WIFSIGNALED(raw_exit_stus))
		{
			object->last_exit_status = 0;
			print_log(imp_stus->stdout_fd, object, \
				"%d (signal exit)\n", object->last_exit_status);
		}
		if (WIFEXITED(raw_exit_stus))
			object->last_exit_status = WEXITSTATUS(raw_exit_stus);
		imp_stus->i += 1;
		print_log(imp_stus->stdout_fd, object, \
			"(부모에서 출력) pid %d : child process exit \n", (int)ret);
	}
	close(imp_stus->stderr_pipe[0]);
}

static void	error_process(t_object *object, t_imp_stus *imp_stus, pid_t ret)
{
	char	buf[4096];
	int		flag;

	(void)object;
	(void)ret;
	flag = read(imp_stus->stderr_pipe[0], buf, sizeof(buf) - 1);
	if (flag <= 0)
	{
		return ;
	}
	buf[flag] = '\0';
	if (ft_strnstr(buf, "Broken pipe", sizeof(buf) - 1))
	{
		print_log(imp_stus->stdout_fd, object, \
			"(%d)(임시 출력) %s\n", (int)ret, buf);
		return ;
	}
	if (ft_strnstr(buf, "write error", sizeof(buf) - 1))
	{
		print_log(imp_stus->stdout_fd, object, \
			"(%d)(임시 출력) %s\n", (int)ret, buf);
		return ;
	}
	printf("Error : %s", buf);
}
