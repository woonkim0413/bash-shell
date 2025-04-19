/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 16:53:47 by rakim             #+#    #+#             */
/*   Updated: 2025/04/19 17:20:11 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	int	is_all_space(const char *line)
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

static	void	check_input(int length, char *input[])
{
	if (length != 1)
		throw_error("argument input error");
	(void)input;
}

int	main(int length, char *input[], char *env[])
{
	char	*line;

	check_input(length, input);
	init_signal();
	while (1)
	{
		line = readline("minishell $ : ");
		if (!line)
			break ;
		if (*line != '\0')
			add_history(line);
		if (*line != '\0' && !is_all_space(line))
		{


		}
		free(line);
	}
	(void)env;
	return (0);
}
