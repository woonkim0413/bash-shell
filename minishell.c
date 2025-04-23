/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 16:53:47 by rakim             #+#    #+#             */
/*   Updated: 2025/04/23 17:42:15 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int length, char *input[], char *env[])
{
	char		*line;
	char		**line_splited_pipe;
	t_object	object;

	init(length, input, &object, env);
	while (1)
	{
		line = readline("minishell $ : ");
		if (!line)
			break ;
		if (*line != '\0')
			add_history(line);
		if (*line != '\0' && !is_all_space(line))
		{
			whitespace_convert_to_space(&line);
			check_double_pipe(&line, &object);
			check_quotes(&line, &object);
			line_splited_pipe = ft_split(line, '|');
			free(line);
			parsing(line_splited_pipe, &object);
		}
		else
			free(line);
	}
	return (0);
}
