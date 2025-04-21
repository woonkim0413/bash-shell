/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:21:30 by rakim             #+#    #+#             */
/*   Updated: 2025/04/21 16:12:59 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	char	*get_env(char *key, t_env *env)
{
	char	*key_name;

	key_name = key + 1;
	while (env)
	{
		if (ft_strlen(env->key) == ft_strlen(key_name) && \
		ft_strncmp(env->key, key_name, ft_strlen(key_name)) == 0)
			return (ft_strdup(env->value));
		env = env->next;
	}
	return (ft_strdup(""));
}

void	handle_env(char **src, int idx, t_env *env)
{
	char	*env_value;

	env_value = get_env(src[idx], env);
	free(src[idx]);
	src[idx] = env_value;
}
