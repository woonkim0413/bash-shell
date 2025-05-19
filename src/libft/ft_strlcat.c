/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 19:57:25 by rakim             #+#    #+#             */
/*   Updated: 2024/10/07 16:48:02 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	dst_idx;
	size_t	src_idx;
	size_t	result;

	dst_idx = ft_strlen(dst);
	src_idx = 0;
	result = 0;
	if (dst_idx < size)
	{
		result += dst_idx;
		while (result < (size) && src[src_idx] != '\0' && dst_idx < (size - 1))
		{
			dst[dst_idx++] = src[src_idx++];
			result++;
		}
		dst[dst_idx] = '\0';
	}
	else
		result += size;
	while (src[src_idx++])
		result++;
	return (result);
}
