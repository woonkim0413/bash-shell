/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 16:04:33 by rakim             #+#    #+#             */
/*   Updated: 2025/04/21 15:46:39 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_all_space(const char *line)
{
	int	idx;

	idx = 0;
	while (line[idx])
	{
		if (line[idx] != 32 && !(line[idx] >= 9 && line[idx] <= 13))
			return (0);
		idx++;
	}
	return (1);
}
