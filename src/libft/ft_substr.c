/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 12:52:36 by rakim             #+#    #+#             */
/*   Updated: 2024/10/19 15:03:19 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*result;
	unsigned int	result_idx;

	if (s == NULL)
		return (NULL);
	if (ft_strlen(s) < start)
	{
		result = (char *)malloc(1);
		result[0] = 0;
		return (result);
	}
	if (ft_strlen(s) < start + len)
		len = ft_strlen(s) - start;
	result = (char *)ft_calloc(len + 1, sizeof(char));
	if (result == NULL)
		return (NULL);
	result_idx = 0;
	while (s[start] && result_idx < len)
		result[result_idx++] = s[start++];
	return (result);
}
