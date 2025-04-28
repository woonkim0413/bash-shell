/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 16:53:47 by rakim             #+#    #+#             */
/*   Updated: 2025/04/28 13:21:04 by woonkim          ###   ########.fr       */
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
			free_all(&object);
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
			parsing(line_splited_by_pipe, &object);
			print_all_cmd(object.cmd_info);
			implement(&object);
		}
	}
	return (0);
}
