/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 15:37:59 by woonkim           #+#    #+#             */
/*   Updated: 2025/05/14 00:13:57 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int execute_pwd(t_object *object, t_imp_stus *imp_stus, int prev_cwd_fd)
{
	char buffer[4096];
	
	(void)object;
	(void)prev_cwd_fd;
	(void)imp_stus;
	getcwd(buffer, sizeof(buffer) - 1);
	printf("%s\n", buffer);
	return (1);
}
