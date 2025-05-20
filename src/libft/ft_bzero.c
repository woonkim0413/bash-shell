/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:39:36 by rakim             #+#    #+#             */
/*   Updated: 2024/10/03 13:52:21 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	size_t			idx;
	unsigned char	*temp;

	idx = 0;
	temp = (unsigned char *)s;
	while (idx < n)
		temp[idx++] = 0;
}
