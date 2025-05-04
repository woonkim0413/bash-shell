/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 21:06:37 by woonkim           #+#    #+#             */
/*   Updated: 2025/04/30 09:16:23 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int execute_echo(t_object *object, t_imp_stus *imp_stus)
{
	char ** argv;
	int		i;
	int		flag;

	// dprintf(imp_stus->stdoutFd, "(자식에서 출력) execute echo\n");
	(void)imp_stus;
	argv = object->cmd_info->evecve_argv;
	flag = 0;
	if (argv[1] && !ft_strncmp(argv[1], "-n", ft_strlen(argv[1])))
			flag = 1;
	i = 0;
	while (argv[++i])
	{
		if (i == 1 && flag == 1)
			continue ;
		// 여기서 pipe broken error가 뜬다 
		// 이미 부모 프로세스에서 pipe 읽기 fd를 close()해서다
		// child가 시작할 때 slgnal함수로 pipe broken을 무시하도록 했다
		write(STDOUT_FILENO, argv[i], ft_strlen(argv[i]));	
		if (argv[i + 1] != NULL)
			write(1, " ", 1);	
	}
	if (!flag)
		write(1, "\n", 1);
	return (1);
}
