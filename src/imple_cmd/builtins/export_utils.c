/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 08:09:05 by woonkim           #+#    #+#             */
/*   Updated: 2025/05/20 12:06:04 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	update_node_extend(t_env *temp, char **argv_equals, \
	int equals_flag);

int	update_node(t_object *object, char **argv_equals, int equals_flag)
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
