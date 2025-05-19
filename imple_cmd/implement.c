/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   implement.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 16:57:12 by woonkim           #+#    #+#             */
/*   Updated: 2025/05/19 10:29:55 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	child_work(t_object *object, t_imp_stus *imp_stus);
static void	parent_while_process(t_object *object, t_imp_stus *imp_stus);
static void	child_while_process(t_object *object, t_imp_stus *imp_stus);
static void implement2(t_object *object, t_imp_stus *imp_stus);

// 명령어와 환경 병수 링크드 리스크로 받음
// t_cmd : 명령어에 대한 meta data를 저장하고 있는 node
// env : 환경 변수를 저장하고 있는 node
// 명령어 없이 파이프라인만 있는 경우
void implement(t_object *object)
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

// fork후 자식 프로세스를 처리하는 함수
static void	child_while_process(t_object *object, t_imp_stus *imp_stus)
{
	int	last_exit_num;

	last_exit_num = 0;
	init_child_signal();
	signal(SIGPIPE, SIG_IGN);
	child_work(object, imp_stus);
	print_log(imp_stus->stdoutFd, object, \
		"(builtin %d) ok (before exit())\n", (int)getpid());
	last_exit_num = object->last_exit_status;
	free_stus_and_object(object, imp_stus);
	exit(last_exit_num);
}

// fork후 부모 프로세스를 처리하는 함수
static void	parent_while_process(t_object *object, t_imp_stus *imp_stus)
{
	close(imp_stus->pipeFd[imp_stus->cur_c_n][1]);
	if (object->cmd_info->prev != NULL)
		close(imp_stus->pipeFd[imp_stus->cur_c_n - 1][0]);
	if (object->cmd_info->next == NULL)
		close(imp_stus->pipeFd[imp_stus->cur_c_n][0]);
	imp_stus->cur_c_n += 1;
	object->cmd_info = object->cmd_info->next;
}

// 자식 process를 명령어에 맞게 execve하여 동작하게 함
// 자식 process의 input과 output fd를 dup2로 적절히 컨트롤 해야함
// 명령어가 builtin이라고 해도 fork()로 처리하는 것이 파이프라인 전체 구조를
// 설계할 때 더 편하다
static void	child_work(t_object *object, t_imp_stus *imp_stus)
{
	char	**envp;
	int 	flag;

	input_output_setting(object, imp_stus, MULTI_PIPLINE);
	if (execute_builtins(object, imp_stus))
		return ;
	single_duuble_quates_check(object);
	flag = find_path(object->cmd_info, object->env);
	create_execve_args(object->cmd_info);
	envp = env_to_char(object->env, imp_stus->i);
	print_log(imp_stus->stdoutFd, object, \
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
