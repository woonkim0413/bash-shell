/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 19:13:47 by rakim             #+#    #+#             */
/*   Updated: 2025/04/23 14:04:59 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	src_idx;
	size_t	dst_idx;

	src_idx = 0;
	dst_idx = 0;
	if (size > 0)
	{
		while (src[src_idx] != '\0' && src_idx < size - 1)
			dst[dst_idx++] = src[src_idx++];
		dst[dst_idx] = '\0';
	}
	while (src[src_idx] != '\0')
		src_idx++;
	return (src_idx);
}
