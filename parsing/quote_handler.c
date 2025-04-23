/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:50:00 by rakim             #+#    #+#             */
/*   Updated: 2025/04/23 19:25:11 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	int	is_have_quotes(char *line)
{
	int	idx;

	idx = 0;
	while (line[idx])
	{
		if (line[idx] == DOUBLE_QUOTE_ASCII)
			return (1);
		if (line[idx] == SINGLE_QUOTE_ASCII)
			return (2);
		idx++;
	}
	return (0);
}

static	int	is_quote_close(char **line, t_object *object)
{
	int		in_duoble;
	int		in_single;
	int		idx;
	char	c;

	in_single = 0;
	in_duoble = 0;
	idx = 0;
	while ((*line)[idx])
	{
		c = (*line)[idx];
		if (c == '\'' && !in_duoble)
			in_single = !in_single;
		else if (c == '"' && !in_single)
			in_duoble = !in_duoble;
		else if (c == '\\' && in_duoble && (*line)[idx + 1] == '"')
			idx++;
		idx++;
	}
	if (in_single || in_duoble)
	{
		free((*line));
		throw_error("quote is not close", object);
		return (0);
	}
	return (1);
}

static	void	single_quotes_handle(char **line, t_object *object)
{
	(void)line;
	(void)object;
}

static	void	double_quotes_handle(char **line, t_object *object)
{
	char	*dollar_location;
	char	*env_value;
	char	*env_key;
	char	*result;
	char	*temp;

	dollar_location = ft_strchr(*line, DOLLAR_ASCII);
	if (!dollar_location || *(dollar_location + 1) == ' ')
		return ;
	env_key = extract_key_in(dollar_location);
	if (env_key == NULL)
		throw_error("syntax error", object);
	env_value = get_env(env_key, object->env);
	result = ft_substr(*line, 0, dollar_location - *line);
	temp = ft_strjoin(result, env_value);
	free(result);
	result = ft_strjoin(temp, dollar_location + ft_strlen(env_key) + 1);
	free(env_key);
	free(*line);
	free(temp);
	*line = result;
	double_quotes_handle(line, object);
}

void	check_quotes(char **line, t_object *object)
{
	char	*quote_start;
	int		quote_marker;

	quote_marker = is_have_quotes(*line);
	if (quote_marker == 0)
		return ;
	if (!is_quote_close(line, object))
		return ;
	if (quote_marker == 1)
	{
		quote_start = ft_strchr(*line, DOUBLE_QUOTE_ASCII);
		double_quotes_handle(line, object);
	}
	if (quote_marker == 2)
	{
		quote_start = ft_strchr(*line, SINGLE_QUOTE_ASCII);
		single_quotes_handle(line, object);
	}
}

// cat "$HOME/Desktop/minishell/minishell.c" 
// | wc -al | echo '"$HOME" is my home' | echo "'$HOME'"