/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   imp_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 20:07:27 by woonkim           #+#    #+#             */
/*   Updated: 2025/04/24 14:59:13 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// builtins : echo, pwd, unset, export, env, exit
// 만약 pwd, echo, env인 경우 출력 값이 있으니 파이프라인을 통해서
// 다음 명령어의 input으로 사용될 수 있음
// 그러니 pipe를 이용하여 data를 buffer넣어주는 것이 좋을 것 같다
void execute_builtins(t_object *object, t_imp_stus *imp_stus)
{
	char *cmd;
	
	cmd = object->cmd_info->cmd;
	if (ft_strcmp("echo", cmd, ft_strlen(cmd)))
		execute_echo(object, imp_stus);
	if (ft_strcmp("pwd", cmd, ft_strlen(cmd)))
		execute_echo(object, imp_stus);
	if (ft_strcmp("unset", cmd, ft_strlen(cmd)))
		execute_echo(object, imp_stus);
	if (ft_strcmp("export", cmd, ft_strlen(cmd)))
		execute_echo(object, imp_stus);
	if (ft_strcmp("env", cmd, ft_strlen(cmd)))
		execute_echo(object, imp_stus);
	if (ft_strcmp("exit", cmd, ft_strlen(cmd)))
		execute_echo(object, imp_stus);
}

// 모든 자식 프로세스 종료 대기 (비정상 종료시 상태 저장)
void wait_childs_process(t_object *object, t_imp_stus *imp_stus)
{
	pid_t ret;
	int status;
	int sig;

	sig = 0;
	while (imp_stus->i < imp_stus->total_c_n)
	{
		// 특정 자식 process가 종료될 때까지 대기, 비정상 종료시 flag저장
		pid_t ret = waitpid(-1, &status, NULL);
		if (ret < 0)
			perror("waitpid error : ");
		// 정상 종료가 아니면
		if (!WIFEXITED(status)) 
		{	// 시그널 종료라면
			if (WIFSIGNALED(status))
			{
				sig = WTERMSIG(status);
				throw_error(strsignal(sig), object);
			}
    	}
		imp_stus->i += 1;
    }
}

char	**env_to_char(t_env *env)
{
	int 	env_num;
	int		i;
	char	*str_temp;
	char 	**str;
	t_env 	*temp;

	temp = env;
	env_num = 0;
	while (temp)
	{
		temp = temp->next;
		env_num ++;
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
