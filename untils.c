/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   untils.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 16:04:33 by rakim             #+#    #+#             */
/*   Updated: 2025/04/20 16:05:29 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_all_space(const char *line)
{
	int	idx;

	idx = 0;
	while (line[idx])
	{
		if (line[idx] != 32 && !(line[idx] >= 9 && line[idx] <= 13))
			return (0);
		idx++;
	}
	return (1);
}

void	print_env_list(t_env *env_list)
{
	while (env_list)
	{
		printf("%s=%s\n", env_list->key, env_list->value);
		env_list = env_list->next;
	}
}
