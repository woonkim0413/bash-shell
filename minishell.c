/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 16:53:47 by rakim             #+#    #+#             */
/*   Updated: 2025/04/20 16:06:03 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int length, char *input[], char *env[])
{
	char	*line;
	t_env	*env_list;

	env_list = init(length, input, env);
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
