/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 15:47:06 by woonkim           #+#    #+#             */
/*   Updated: 2025/05/19 14:24:12 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	check_extend(t_object *object, int *flag);

int	check_one_builtin(t_object *object, t_imp_stus *imp_stus)
{
	char	*cmd;
	int		flag;

	(void)imp_stus;
	flag = 0;
	if (object->cmd_info->prev == NULL && object->cmd_info->next == NULL)
	{
		cmd = object->cmd_info->cmd;
		if (!cmd || ft_strlen(cmd) == 0)
			flag = 0;
		else if (!ft_strncmp("echo", cmd, 4) && \
			ft_strlen("echo") == ft_strlen(cmd))
			flag = 1;
		else if (!ft_strncmp("pwd", cmd, 3) && \
			ft_strlen("pwd") == ft_strlen(cmd))
			flag = 1;
		check_extend(object, &flag);
	}
	return (flag);
}

static void	check_extend(t_object *object, int *flag)
{
	char	*cmd;

	cmd = object->cmd_info->cmd;
	if (!ft_strncmp("unset", cmd, 5) && \
		ft_strlen("unset") == ft_strlen(cmd))
		*flag = 1;
	else if (!ft_strncmp("export", cmd, 6) && \
		ft_strlen("export") == ft_strlen(cmd))
		*flag = 1;
	else if (!ft_strncmp("env", cmd, 3) && \
		ft_strlen("env") == ft_strlen(cmd))
		*flag = 1;
	else if (!ft_strncmp("exit", cmd, 4) && \
		ft_strlen("exit") == ft_strlen(cmd))
		*flag = 1;
	else if (!ft_strncmp("cd", cmd, 2) && \
		ft_strlen("cd") == ft_strlen(cmd))
		*flag = 1;
}

//  파이프라인에서 builtin을 사용할 때 실행됨 ; fork()후 자식 프로세스에서 실행됨
int	execute_builtins(t_object *object, t_imp_stus *imp_stus)
{
	int		flag;
	char	*cmd;

	flag = 0;
	cmd = object->cmd_info->cmd;
	if (!cmd || ft_strlen(cmd) == 0)
		flag = 0;
	else if (!ft_strncmp("echo", cmd, 4) && ft_strlen("echo") == ft_strlen(cmd))
		flag = execute_echo(object, imp_stus);
	else if (!ft_strncmp("pwd", cmd, 3) && ft_strlen("pwd") == ft_strlen(cmd))
		flag = execute_pwd(object, imp_stus, -1);
	else if (!ft_strncmp("unset", cmd, 5) && \
		ft_strlen("unset") == ft_strlen(cmd))
		flag = execute_unset(object, imp_stus);
	else if (!ft_strncmp("export", cmd, 6) && \
		ft_strlen("export") == ft_strlen(cmd))
		flag = execute_export(object, imp_stus);
	else if (!ft_strncmp("env", cmd, 3) && ft_strlen("env") == ft_strlen(cmd))
		flag = execute_env(object, imp_stus);
	else if (!ft_strncmp("exit", cmd, 4) && ft_strlen("exit") == ft_strlen(cmd))
		flag = execute_exit(object, imp_stus);
	else if (!ft_strncmp("cd", cmd, 2) && ft_strlen("cd") == ft_strlen(cmd))
		flag = execute_cd(object, imp_stus);
	return (flag);
}

// * builtin 단독 사용시 실행됨 ; 부모 프로세스에서 실행됨
void	execute_one_builtin(t_object *object, t_imp_stus *imp_stus)
{
	char	*cmd;

	if (!imp_stus->all_path)
	{
		cmd = object->cmd_info->cmd;
		if (!ft_strncmp("echo", cmd, ft_strlen(cmd)))
			execute_echo(object, imp_stus);
		else if (!ft_strncmp("pwd", cmd, ft_strlen(cmd)))
			execute_pwd(object, imp_stus, -1);
		else if (!ft_strncmp("unset", cmd, ft_strlen(cmd)))
			execute_unset(object, imp_stus);
		else if (!ft_strncmp("export", cmd, ft_strlen(cmd)))
			execute_export(object, imp_stus);
		else if (!ft_strncmp("env", cmd, ft_strlen(cmd)))
			execute_env(object, imp_stus);
		else if (!ft_strncmp("exit", cmd, ft_strlen(cmd)))
			execute_exit(object, imp_stus);
		else if (!ft_strncmp("cd", cmd, ft_strlen(cmd)))
			execute_cd(object, imp_stus);
	}
	dup2(imp_stus->stdout_fd, STDOUT_FILENO);
	dup2(imp_stus->stdin_fd, STDIN_FILENO);
}

void	printf_env(t_object *object)
{
	t_env	*temp;

	temp = object->env;
	while (temp)
	{
		printf("declare -x %s", temp->key);
		if (temp->value)
			printf("=\"%s\"", temp->value);
		printf("\n");
		temp = temp->next;
	}
	return ;
}
