/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <rakim@student.42gyeongsan.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 14:06:48 by rakim             #+#    #+#             */
/*   Updated: 2024/10/20 16:29:58 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t			idx;
	unsigned char	*t_dest;
	unsigned char	*t_src;

	if (dest == NULL && src == NULL)
		return (NULL);
	idx = 0;
	t_dest = (unsigned char *)dest;
	t_src = (unsigned char *)src;
	while (idx < n)
	{
		t_dest[idx] = t_src[idx];
		idx++;
	}
	return (dest);
}
