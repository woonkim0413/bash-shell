/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 12:32:46 by woonkim           #+#    #+#             */
/*   Updated: 2025/05/01 19:33:00 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int execute_exit(t_object *object, t_imp_stus *imp_stus)
{
	free_stus_and_object(object, imp_stus);
	exit(0);
}