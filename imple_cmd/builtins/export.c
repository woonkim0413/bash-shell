/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 12:33:32 by woonkim           #+#    #+#             */
/*   Updated: 2025/05/19 13:07:57 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_export_extend(t_object *object, t_imp_stus *imp_stus, \
	char **evecve_arg, int equals_flag);
static void	insert_node(t_object *object, char **argv, int equals_flag);
static int	update_node(t_object *object, char **argv_equals, int equals_flag);
static void	update_node_extend(t_env *temp, char **argv_equals, \
	int equals_flag);

int	execute_export(t_object *object, t_imp_stus *imp_stus)
{
	char	**evecve_arg;
	int		equals_flag;

	if (!object->cmd_info->evecve_argv[1])
	{
		printf_env(object);
		return (1);
	}
	evecve_arg = object->cmd_info->evecve_argv;
	imp_stus->i = 0;
	while (evecve_arg[++(imp_stus->i)])
	{
		equals_flag = 0;
		if (evecve_arg[(imp_stus->i)][0] == '=')
		{
			write(imp_stus->stdoutFd, "Error : export: '=': ", 21); 
			write(imp_stus->stdoutFd, "not a valiid identifler\n", 25);
			object->last_exit_status = 1;
			continue ;
		}
		execute_export_extend(object, imp_stus, evecve_arg, equals_flag);
	}
	return (1);
}

static void	execute_export_extend(t_object *object, t_imp_stus *imp_stus, \
	char **evecve_arg, int equals_flag)
{
	char	**argv_equals;

	if (ft_strnstr(evecve_arg[imp_stus->i], "=", \
		ft_strlen(evecve_arg[imp_stus->i])))
		equals_flag = 1;
	argv_equals = ft_split(evecve_arg[imp_stus->i], '=');
	if (!update_node(object, argv_equals, equals_flag))
		insert_node(object, argv_equals, equals_flag);
	free_doublechar(argv_equals);
}

static int	update_node(t_object *object, char **argv_equals, int equals_flag)
{
	t_env	*temp;
	int		temp_len;
	int		argv_len;

	temp = object->env;
	argv_len = ft_strlen(argv_equals[0]);
	while (temp)
	{
		temp_len = ft_strlen(temp->key);
		if (!ft_strncmp(temp->key, argv_equals[0], temp_len) && \
			(temp_len == argv_len))
		{
			update_node_extend(temp, argv_equals, equals_flag);
			return (1);
		}
		temp = temp->next;
	}
	return (0);
}

static void	update_node_extend(t_env *temp, char **argv_equals, \
	int equals_flag)
{
	if (temp->value)
		free(temp->value);
	if (argv_equals[1])
		temp->value = ft_strdup(argv_equals[1]);
	else if (equals_flag)
	{
		temp->value = (char *)malloc(sizeof(char));
		temp->value[0] = '\0';
	}
	else
		temp->value = NULL;
}

static void	insert_node(t_object *object, char **argv, int equals_flag)
{
	t_env	*temp;
	t_env	*env;

	env = (t_env *)malloc(sizeof(t_env));
	temp = object->env;
	while (temp->next)
		temp = temp->next;
	env->key = ft_strdup(argv[0]);
	if (argv[1])
		env->value = ft_strdup(argv[1]);
	else if (equals_flag)
	{
		env->value = (char *)malloc(sizeof(char));
		env->value[0] = '\0';
	}
	else
		env->value = NULL;
	env->next = NULL;
	temp->next = env;
}

