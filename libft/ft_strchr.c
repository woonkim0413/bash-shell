/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <rakim@student.42gyeongsan.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 14:19:35 by rakim             #+#    #+#             */
/*   Updated: 2024/10/20 16:16:35 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int	idx;
	int	s_len;

	idx = 0;
	s_len = ft_strlen(s);
	while (idx < s_len)
	{
		if (s[idx] == (char)c)
			return ((char *)(s + idx));
		idx++;
	}
	if (s[idx] == (char)c)
		return ((char *)&s[idx]);
	return (0);
}
