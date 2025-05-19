/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <rakim@student.42gyeongsan.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 14:12:43 by rakim             #+#    #+#             */
/*   Updated: 2024/10/20 15:12:03 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

void	*ft_memset(void *s, int c, size_t n)
{
	size_t			idx;
	unsigned char	*ptr;

	ptr = (unsigned char *)s;
	idx = 0;
	while (idx < n)
		ptr[idx++] = (unsigned char)c;
	return (s);
}
