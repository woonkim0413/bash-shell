/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 12:34:55 by woonkim           #+#    #+#             */
/*   Updated: 2025/05/02 00:48:33 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void env_remove(t_env *env)
{
	free(env->key);
	free(env->value);
	env->next = NULL;
	free(env);
}

static void remove_env(char *remove_target, t_env *pre, t_env *cur)
{
	while (cur)
	{
		if (!ft_strncmp(remove_target, cur->key, ft_strlen(cur->key)))
		{
			pre->next = cur->next;
			env_remove(cur);
		}
		cur = cur->next;
		pre = pre->next;
	}
}

int execute_unset(t_object *object, t_imp_stus *imp_stus)
{
	t_env 	*pre;
	t_env 	*cur;
	char	*remove_target;

	(void)imp_stus;
	remove_target = object->cmd_info->evecve_argv[1];
	pre = object->env;
	cur = object->env->next;
	if (!ft_strncmp(remove_target, pre->key, ft_strlen(pre->key)))
	{
		object->env = cur;
		env_remove(pre);
		return (1);
	}
	remove_env(remove_target, pre, cur);
	return (1);
}

