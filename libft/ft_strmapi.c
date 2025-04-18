/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <rakim@student.42gyeongsan.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 17:16:29 by rakim             #+#    #+#             */
/*   Updated: 2024/10/20 15:53:27 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	size_t			s_len;
	unsigned int	s_idx;
	char			*result;

	if (!s || !f)
		return (NULL);
	s_len = ft_strlen(s);
	result = (char *)ft_calloc(s_len + 1, sizeof(char));
	if (result == NULL)
		return (NULL);
	s_idx = 0;
	while (s[s_idx])
	{
		result[s_idx] = f(s_idx, s[s_idx]);
		s_idx++;
	}
	return (result);
}
