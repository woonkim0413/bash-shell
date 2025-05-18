/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 12:26:37 by rakim             #+#    #+#             */
/*   Updated: 2025/05/16 23:57:30 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strdup(const char *s)
{
	size_t	len;
	char	*result;
	size_t	idx;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	idx = 0;
	result = (char *)ft_calloc(len + 1, sizeof(char));
	if (result == NULL)
		return (NULL);
	while (s[idx] && idx < len)
	{
		result[idx] = s[idx];
		idx++;
	}
	return (result);
}
