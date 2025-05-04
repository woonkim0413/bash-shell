/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 12:33:32 by woonkim           #+#    #+#             */
/*   Updated: 2025/05/04 23:24:22 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void printf_env(t_object *object);
static void insert_node(t_object *object, char **argv);

// TODO : 만약, 기존에 있는 key값과 동일한 경우 update해야한다
// TODO : export aaa=aaaaaaa kkk=aaaaaaaa ttt=aaaaaaa hhhhhhh
// 이런 경우에 모든 목록을 등록해야 한다.... 시발
int	execute_export(t_object *object, t_imp_stus *imp_stus)
{
	char 	**argv;
	int		i;

	(void)imp_stus;
	// export만 실행
	if (!object->cmd_info->evecve_argv[1])
	{
		printf_env(object);
		return (1);
	}
	// env node 추가
	argv = ft_split(object->cmd_info->evecve_argv[1], ' ');
	i = 0;
	while (argv[++i])
		free(argv[i]);
	argv = ft_split(argv[0], '=');
	insert_node(object, argv);
	return (1);
}

static void insert_node(t_object *object, char **argv)
{
	t_env	*temp;
	t_env 	*env;

	env = (t_env *)malloc(sizeof(t_env));
	temp = object->env;
	while (temp->next)
		temp = temp->next;
	env->key = argv[0];
	if (argv[1])
		env->value = argv[1];
	else
		env->value = NULL;
	env->next = NULL;
	temp->next = env;
}

static void printf_env(t_object *object)
{
	t_env *temp;

	temp = object->env;
	while (temp)
	{
		printf("declare -x %s", temp->key);
		if (temp->value)
			printf("=%s", temp->value);
		printf("\n");
		temp = temp->next;
	}
	return ;
}
