/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 21:06:37 by woonkim           #+#    #+#             */
/*   Updated: 2025/04/27 15:37:49 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void execute_echo(t_object *object, t_imp_stus *imp_stus)
{
	char ** argv;
	int		i;
	int		flag;

	argv = object->cmd_info->evecve_argv;
	flag = 0;
	if (argv[1] && !ft_strncmp(argv[1], "-n", ft_strlen(argv[1])))
			flag = 1;
	i = 0;
	while (argv[++i])
	{
		if (i == 1 && flag == 1)
			continue ;
		write(1, argv[i], ft_strlen(argv[i]));	
		if (argv[i + 1] != NULL)
			write(1, " ", 1);	
	}
	if (!flag)
		write(1, "\n", 1);
}
