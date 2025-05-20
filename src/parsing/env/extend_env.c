/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extend_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:21:30 by rakim             #+#    #+#             */
/*   Updated: 2025/05/13 15:49:02 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static	char	*make_new_line(char **old_line, char *dollar_location, \
	char **env_value, int key_len)
{
	char	*prefix;
	char	*suffix;
	char	*temp;
	char	*result;
	int		prefix_len;

	prefix_len = dollar_location - *old_line;
	prefix = ft_substr(*old_line, 0, prefix_len);
	suffix = ft_strdup(dollar_location + key_len);
	temp = ft_strjoin(prefix, *env_value);
	result = ft_strjoin(temp, suffix);
	free(prefix);
	free(suffix);
	free(temp);
	free(*env_value);
	free(*old_line);
	return (result);
}

static	void	handle_question_mark(int last_exit_status, \
	char **env_value, int *key_len)
{
	*env_value = ft_itoa(last_exit_status);
	*key_len = 2;
}

static	void	set_env_value_and_key(char **env_value, char **env_key, \
t_object *object, int *key_len)
{
	*env_value = get_env(*env_key, object->env);
	*key_len = ft_strlen(*env_key);
	free(*env_key);
}

void	extend_env(char **line, int *dollar_idx, t_object *object)
{
	char	*dollar_location;
	char	*env_value;
	char	*env_key;
	int		key_len;
	char	*old_line;

	dollar_location = *line + *dollar_idx;
	old_line = *line;
	if (*(dollar_location + 1) == ' ')
		return ;
	if (*(dollar_location + 1) == '?')
		handle_question_mark(object->last_exit_status, &env_value, &key_len);
	else
	{
		env_key = extract_key_in(dollar_location);
		if (env_key == NULL)
		{
			throw_error("syntax error", object, NULL, line);
			return ;
		}
		set_env_value_and_key(&env_value, &env_key, object, &key_len);
	}
	*dollar_idx += (ft_strlen(env_value) - 1);
	*line = make_new_line(&old_line, dollar_location, &env_value, key_len);
}
