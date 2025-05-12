/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 16:53:47 by rakim             #+#    #+#             */
/*   Updated: 2025/05/12 18:48:20 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int length, char *input[], char *env[])
{
	char		*line;
	char		**line_splited_by_pipe;
	t_object	object;

	init(length, input, &object, env);
	while (1)
	{
		line = readline("minishell $ : ");
		if (!line)
		{
			free(line);
			free_object(&object);
			break ;
		}
		if (*line != '\0')
			add_history(line);
		if (*line != '\0' && !is_all_space(line))
		{
			whitespace_convert_to_space(&line);
			check_pipe(&line, &object);
			check_quotes(&line, &object);
			line_splited_by_pipe = extend_env_and_split(&line, &object);
			free(line);
			parsing(line_splited_by_pipe, &object);
			handle_heardoc(&object);
			// handle_empty_pipe(&object);
			print_all_cmd(object.cmd_info);
			implement(&object);
		}
	}
	return (0);
}
