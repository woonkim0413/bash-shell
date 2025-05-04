/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   imp_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 20:07:27 by woonkim           #+#    #+#             */
/*   Updated: 2025/05/04 15:26:48 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int argv_end_check(char *argv);

// builtins : echo, pwd, unset, export, env, exit
// pwd, echo, env인 경우 출력 값이 있으니 파이프라인을 통해서
// 다음 명령어의 input으로 사용될 수 있음
// * builtin 단독 사용 : 부모 프로세스에서 실행됨
//   파이프라인에서 builtin 사용 : fork()후 자식 프로세스에서 실행됨
int execute_builtins(t_object *object, t_imp_stus *imp_stus)
{
	int flag;
	char *cmd;

	flag = 0;
	cmd = object->cmd_info->cmd;
	if (!ft_strncmp("echo", cmd, ft_strlen(cmd)))
		flag = execute_echo(object, imp_stus);
	else if (!ft_strncmp("pwd", cmd, ft_strlen(cmd)))
		flag = execute_pwd(object, imp_stus, -1);
	else if (!ft_strncmp("unset", cmd, ft_strlen(cmd)))
		flag = execute_unset(object, imp_stus);
	else if (!ft_strncmp("export", cmd, ft_strlen(cmd)))
		flag = execute_export(object, imp_stus);
	else if (!ft_strncmp("env", cmd, ft_strlen(cmd)))
		flag = execute_env(object, imp_stus);
	else if (!ft_strncmp("exit", cmd, ft_strlen(cmd)))
		flag = execute_exit(object, imp_stus);
	else if (!ft_strncmp("cd", cmd, ft_strlen(cmd)))
		flag = execute_cd(object, imp_stus);
	if (flag)
	{
		// 현재 파이프 buffer 쓰기 fd를 STDOUT으로 재연결 한 뒤에 명령어 실행 후 close한다
		// close(STDOUT_FILENO);
		// 이전 파이프 buffer 읽기 fd를 STDIN으로 재연결 한 뒤에 명령어 실행 후 close한다 
		// close(STDIN_FILENO);
	}
	return (flag);
}

// 모든 자식 프로세스 종료 대기 (비정상 종료시 상태 저장)
// 비정상 종료하는 경우 여기서 error메세지를 terminal로 출력
void wait_childs_process(t_object *object, t_imp_stus *imp_stus)
{
	pid_t ret;
	int code;

	while (imp_stus->i < imp_stus->total_c_n)
	{
		// printf("%d번 명령어 : before waitpid()\n", imp_stus->i);
		// 특정 자식 process가 종료될 때까지 대기, 비정상 종료시 flag저장
		ret = waitpid(-1, &(object->last_exit_status), 0);
		// printf("%d번 명령어 : after waitpid()\n", imp_stus->i);
		if (ret < 0)
			perror("waitpid error : ");
		//시그널 종료 확인
		if (WIFSIGNALED(object->last_exit_status))
			printf("%d 시그널 종료\n", WTERMSIG(object->last_exit_status));
		// 자식 프로세스가 정상 종료됐는지 확인 (exit(), return 종료)
		if (WIFEXITED(object->last_exit_status))
		{
			// 종료 상태 값 확인
			code = WEXITSTATUS(object->last_exit_status);
			if (code)
			{
				printf("(%d)(임시출력) EXIT number : %d\n", (int)ret, WEXITSTATUS(object->last_exit_status));
				error_process(object, imp_stus, ret);	
			}
		}
		imp_stus->i += 1;
		printf("(부모에서 출력) pid %d : child process exit \n", (int)ret);
	}
	// 에러 출력 후에 읽기 파이프도 닫아줌
	close(imp_stus->stderr_pipe[0]);
}

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
			free_doublechar(argv);
			return;
		}
	}
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
