/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 19:41:16 by rakim             #+#    #+#             */
/*   Updated: 2025/04/26 19:41:37 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env(char *key, t_env *env)
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
