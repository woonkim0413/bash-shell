/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 15:47:27 by rakim             #+#    #+#             */
/*   Updated: 2025/04/20 16:05:21 by rakim            ###   ########.fr       */
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

static	t_env	*env_string_arr_to_env_list(char *string_arr[])
{
	t_env	*head;
	t_env	*tail;
	t_env	*node;
	int		idx;

	idx = 0;
	head = NULL;
	while (string_arr[idx])
	{
		node = malloc(sizeof(t_env));
		if (!node)
			throw_error("malloc error", head);
		make_key_value_in_list(node, string_arr[idx]);
		node->next = NULL;
		if (!head)
			head = node;
		else
			tail->next = node;
		tail = node;
		idx++;
	}
	print_env_list(head);
	return (head);
}

static	void	check_input(int length, char *input[])
{
	if (length != 1)
		throw_error("argument input error", NULL);
	(void)input;
}

t_env	*init(int length, char *input[], char *env[])
{
	check_input(length, input);
	init_signal();
	return (env_string_arr_to_env_list(env));
}
