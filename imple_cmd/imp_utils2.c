/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   imp_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 20:07:27 by woonkim           #+#    #+#             */
/*   Updated: 2025/04/28 18:09:51 by woonkim          ###   ########.fr       */
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
		flag = execute_pwd(object, imp_stus);
	else if (!ft_strncmp("unset", cmd, ft_strlen(cmd)))
		flag = execute_unset(object, imp_stus);
	else if (!ft_strncmp("export", cmd, ft_strlen(cmd)))
		flag = execute_export(object, imp_stus);
	else if (!ft_strncmp("env", cmd, ft_strlen(cmd)))
		flag = execute_env(object, imp_stus);
	else if (!ft_strncmp("exit", cmd, ft_strlen(cmd)))
		flag = execute_exit(object, imp_stus);
	return (flag);
}

// 모든 자식 프로세스 종료 대기 (비정상 종료시 상태 저장)
// 비정상 종료하는 경우 여기서 error메세지를 terminal로 출력
void wait_childs_process(t_object *object, t_imp_stus *imp_stus)
{
	pid_t ret;
	int sig;

	sig = 0;
	while (imp_stus->i < imp_stus->total_c_n)
	{
		// 특정 자식 process가 종료될 때까지 대기, 비정상 종료시 flag저장
		ret = waitpid(-1, &(object->last_exit_status), 0);
		if (ret < 0)
			perror("waitpid error : ");
		// 정상 종료가 아니면
		if (!WIFEXITED(object->last_exit_status))
		{ // 시그널 종W료라면
			if (WIFSIGNALED(object->last_exit_status))
			{
				sig = WTERMSIG(object->last_exit_status);
				throw_error(strsignal(sig), object, imp_stus);
			}
		}
		imp_stus->i += 1;
		printf("(부모에서 출력) pid %d : child process exit \n", (int)ret);
	}
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
				cmd_info->evecve_argv[k++] = argv[i++];
			cmd_info->evecve_argv[k] = NULL;
			free(argv);
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
