/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 15:47:27 by rakim             #+#    #+#             */
/*   Updated: 2025/04/26 14:34:23 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	void	make_key_value_in_list(t_env *node, char *origin)
{
	char	*equal;

	equal = ft_strchr(origin, '=');
	if (equal)
	{
		node->key = ft_substr(origin, 0, equal - origin);
		node->value = ft_strdup(equal + 1);
	}
	else
	{
		node->key = ft_strdup(origin);
		node->value = ft_strdup("");
	}
}

static	void	env_string_arr_to_env_list(char *string_arr[], \
	t_object *object)
{
	t_env	*tail;
	t_env	*node;
	int		idx;

	idx = 0;
	while (string_arr[idx])
	{
		node = malloc(sizeof(t_env));
		if (!node)
			throw_error("malloc error", object);
		make_key_value_in_list(node, string_arr[idx]);
		node->next = NULL;
		if (!object->env)
			object->env = node;
		else
			tail->next = node;
		tail = node;
		idx++;
	}
}

static	void	check_input(int length, char *input[])
{
	if (length != 1)
		throw_error("argument input error", NULL);
	(void)input;
}

void	init(int length, char *input[], t_object *object, char **env)
{
	object->env = NULL;
	object->cmd_info = NULL;
	object->env = 0;
	check_input(length, input);
	init_signal();
	env_string_arr_to_env_list(env, object);
}
