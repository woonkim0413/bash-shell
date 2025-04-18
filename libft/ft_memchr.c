/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <rakim@student.42gyeongsan.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 15:04:04 by rakim             #+#    #+#             */
/*   Updated: 2024/10/20 15:10:16 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*temp;
	size_t			idx;

	temp = (unsigned char *)s;
	idx = 0;
	while (idx < n)
	{
		if (temp[idx] == (unsigned char)c)
			return ((void *)(temp + idx));
		idx++;
	}
	return (NULL);
}
