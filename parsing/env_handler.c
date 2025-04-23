/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:21:30 by rakim             #+#    #+#             */
/*   Updated: 2025/04/23 19:03:03 by rakim            ###   ########.fr       */
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

void	handle_dollar(char **src, int idx, t_env *env)
{
	char	*env_value;

	env_value = get_env(src[idx], env);
	free(src[idx]);
	src[idx] = env_value;
}

char	*extract_key_in(char *src)
{
	int		idx;
	char	*result;

	idx = 1;
	if (!ft_isalnum(src[idx]) && src[idx] != '_')
		return (NULL);
	while (src[idx] && (ft_isalnum(src[idx]) || src[idx] == '_'))
		idx++;
	result = ft_calloc(sizeof(char), idx + 1);
	if (!result)
		return (NULL);
	ft_strlcat(result, src, idx + 1);
	return (result);
}
