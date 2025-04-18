/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 17:11:24 by rakim             #+#    #+#             */
/*   Updated: 2024/10/25 14:34:12 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <limits.h>

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t			total_len;
	unsigned char	*result;
	size_t			idx;

	if (nmemb != 0 && size > size - 1 / nmemb)
		return (NULL);
	total_len = nmemb * size;
	result = (unsigned char *)malloc(total_len);
	if (result == NULL)
		return (NULL);
	idx = 0;
	while (idx < total_len)
		result[idx++] = 0;
	return ((void *)result);
}
