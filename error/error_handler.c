/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 16:00:12 by rakim             #+#    #+#             */
/*   Updated: 2025/05/12 14:17:03 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	throw_error(char *message, t_object *object, t_imp_stus *imp_stus)
{
	if (object)
		free_object(object);
	if (imp_stus)
		free_stus(imp_stus);
	printf("Error : %s\n", message);
	exit(1);
}
