/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 12:33:32 by woonkim           #+#    #+#             */
/*   Updated: 2025/05/20 09:56:47 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_export_extend(t_object *object, t_imp_stus *imp_stus, \
	char **evecve_arg, int equals_flag);
static void	insert_node(t_object *object, char **argv, int equals_flag);
static int check_export_error(t_object *object, t_imp_stus *imp_stus, \
	char **evecve_arg);

int	execute_export(t_object *object, t_imp_stus *imp_stus)
{
	char	**evecve_arg;
	int		equals_flag;
	int		error_flag;

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
		error_flag = check_export_error(object, imp_stus, evecve_arg);
		if (error_flag)
			continue ;
		execute_export_extend(object, imp_stus, evecve_arg, equals_flag);
	}
	return (1);
}

static int check_export_error(t_object *object, t_imp_stus *imp_stus, char **evecve_arg)
{
	int		i;
	char	*str;

	i = 0;
	str = evecve_arg[(imp_stus->i)]; 
	if (str[i] == '=')
	{
		write(imp_stus->stdout_fd, "Error : export: '=': ", 21);
		write(imp_stus->stdout_fd, "not a valiid identifler\n", 25);
		object->last_exit_status = 1;
		return (1);
	}
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalpha(str[i]) && str[i] != '_')
		{
			write(imp_stus->stdout_fd, "Error : export: '=': ", 21);
			write(imp_stus->stdout_fd, "not a valiid identifler\n", 25);
			object->last_exit_status = 1;
			return (1);
		}
		i ++;
	}
	return (0);
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
