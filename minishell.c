/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 16:53:47 by rakim             #+#    #+#             */
/*   Updated: 2025/05/18 18:15:56 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	void	process_minishell(t_object *object, char **line)
{
	char	**line_splited_by_pipe;

	whitespace_convert_to_space(line);
	check_pipe(line, object);
	check_quotes(line, object);
	line_splited_by_pipe = extend_env_and_split(line, object);
	parsing(line_splited_by_pipe, object);
	handle_heardoc(object);
	clean_up_quote(object->cmd_info);
	print_all_cmd(object);
	if (object->cmd_info)
		implement(object);
}

static	int	check_heredoc_interrupt(t_object *object)
{
	if (object->heredoc_interrupted)
	{
		object->heredoc_interrupted = 0;
		return (1);
	}
	return (0);
}

int	main(int length, char *input[], char *env[])
{
	char		*line;
	t_object	object;

	init(length, input, &object, env);
	while (1)
	{
		if (check_heredoc_interrupt(&object))
			continue ;
		line = readline("minishell $ : ");
		if (!line)
		{
			free(line);
			free_object(&object);
			clear_history();
			break ;
		}
		if (*line != '\0')
			add_history(line);
		if (*line != '\0' && !is_all_space(line))
			process_minishell(&object, &line);	
		free(line);
		free_cmd_info(&(object.cmd_info));
	}
	return (0);
}
