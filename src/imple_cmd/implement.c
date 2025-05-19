/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   implement.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 16:57:12 by woonkim           #+#    #+#             */
/*   Updated: 2025/05/19 15:27:24 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	child_work(t_object *object, t_imp_stus *imp_stus);
static void	parent_while_process(t_object *object, t_imp_stus *imp_stus);
static void	child_while_process(t_object *object, t_imp_stus *imp_stus);
static void	implement2(t_object *object, t_imp_stus *imp_stus);

void	implement(t_object *object)
{
	t_imp_stus	imp_stus;
	int			one_builtin_flag;	

	print_log(1, object, "\n------------------ 구현부 ---------------------\n\n");
	setting_pipline(object->cmd_info, &imp_stus);
	one_builtin_flag = check_one_builtin(object, &imp_stus);
	if (one_builtin_flag)
	{	
		create_execve_args(object->cmd_info);
		input_output_setting(object, &imp_stus, one_builtin_flag);
		execute_one_builtin(object, &imp_stus);
		free_stus(&imp_stus);
		print_log(1, object, "(부모에서 실행) complete one builtin execution\n");
		return ;
	}
	print_log(1, object, "total cmd : %d \n", imp_stus.total_c_n);
	implement2(object, &imp_stus);
	wait_childs_process(object, &imp_stus);
	object->cmd_info = imp_stus.existing_cmd_info;
	free_stus(&imp_stus);
	return ;
}

static void	implement2(t_object *object, t_imp_stus *imp_stus)
{
	char	*buf;

	buf = "(부모에서 출력) pid %d : create child process ok\n";
	while (imp_stus->cur_c_n < imp_stus->total_c_n)
	{
		pipe_and_fork(imp_stus);
		if (imp_stus->chil_pid[imp_stus->cur_c_n] != 0)
			print_log(1, object, buf, imp_stus->chil_pid[imp_stus->cur_c_n]);
		if (imp_stus->chil_pid[imp_stus->cur_c_n] == 0)
			child_while_process(object, imp_stus);
		else
			parent_while_process(object, imp_stus);
	}
	close(imp_stus->stderr_pipe[1]);
}

static void	child_while_process(t_object *object, t_imp_stus *imp_stus)
{
	int	last_exit_num;

	last_exit_num = 0;
	init_child_signal();
	signal(SIGPIPE, SIG_IGN);
	child_work(object, imp_stus);
	print_log(imp_stus->stdout_fd, object, \
		"(builtin %d) ok (before exit())\n", (int)getpid());
	last_exit_num = object->last_exit_status;
	free_stus_and_object(object, imp_stus);
	exit(last_exit_num);
}

static void	parent_while_process(t_object *object, t_imp_stus *imp_stus)
{
	close(imp_stus->pipe_fd[imp_stus->cur_c_n][1]);
	if (object->cmd_info->prev != NULL)
		close(imp_stus->pipe_fd[imp_stus->cur_c_n - 1][0]);
	if (object->cmd_info->next == NULL)
		close(imp_stus->pipe_fd[imp_stus->cur_c_n][0]);
	imp_stus->cur_c_n += 1;
	object->cmd_info = object->cmd_info->next;
}

static void	child_work(t_object *object, t_imp_stus *imp_stus)
{
	char	**envp;
	int		flag;

	input_output_setting(object, imp_stus, MULTI_PIPLINE);
	if (execute_builtins(object, imp_stus))
		return ;
	single_duuble_quates_check(object);
	flag = find_path(object->cmd_info, object->env);
	create_execve_args(object->cmd_info);
	envp = env_to_char(object->env, imp_stus->i);
	print_log(imp_stus->stdout_fd, object, \
		"(실행파일 %d) ok (before execve)\n", (int)getpid());
	if (!flag || execve(object->cmd_info->cmd_path, \
		object->cmd_info->evecve_argv, envp) == -1)
	{
		close(STDIN_FILENO);
		free_stus_and_object(object, imp_stus);
		free_doublechar(envp);
		exit(127);
	}
}
