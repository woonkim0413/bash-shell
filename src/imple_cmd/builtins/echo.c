/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 21:06:37 by woonkim           #+#    #+#             */
/*   Updated: 2025/05/19 14:21:05 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_dash_flag(char **argv, int *flag, int i);

int	execute_echo(t_object *object, t_imp_stus *imp_stus)
{
	char	**argv;
	int		i;
	int		flag;

	(void)imp_stus;
	argv = object->cmd_info->evecve_argv;
	flag = 0;
	i = 0;
	check_dash_flag(argv, &flag, i);
	while (argv[++i])
	{
		if (i == 1 && flag == 1)
			continue ;
		write(STDOUT_FILENO, argv[i], ft_strlen(argv[i]));
		if (argv[i + 1] != NULL)
			write(1, " ", 1);
	}
	if (!flag)
		write(1, "\n", 1);
	object->last_exit_status = 0;
	return (1);
}

void	check_dash_flag(char **argv, int *flag, int i)
{
	i = 0;
	if (argv[1] && argv[1][0] == '-')
	{
		while (argv[1][++i])
		{
			if (argv[1][i] == 'n')
			{
				*flag = 1;
				continue ;
			}
			else
			{
				*flag = 0;
				break ;
			}
		}
	}
}
