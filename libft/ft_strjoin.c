/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 14:21:43 by rakim             #+#    #+#             */
/*   Updated: 2024/10/06 14:38:37 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	s1_len;
	size_t	s2_len;
	char	*result;
	int		result_idx;
	int		original_idx;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	result = (char *)malloc(s1_len + s2_len + 1);
	if (result == NULL)
		return (NULL);
	result_idx = 0;
	original_idx = 0;
	while (s1[original_idx])
		result[result_idx++] = s1[original_idx++];
	original_idx = 0;
	while (s2[original_idx])
		result[result_idx++] = s2[original_idx++];
	result[result_idx] = '\0';
	return (result);
}
