/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 13:44:40 by woonkim           #+#    #+#             */
/*   Updated: 2025/02/25 09:22:16 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	ft_strlen2(char *str)
{
	int	count;

	count = 0;
	while (*str++)
		count ++;
	return (count);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	int		s1_len;
	int		s2_len;
	int		i;

	if (s1 == 0 || s2 == 0)
		return (NULL);
	s1_len = ft_strlen2((char *)s1);
	s2_len = ft_strlen2((char *)s2);
	str = (char *)malloc(s1_len + s2_len + 1);
	if (str == 0)
		return (NULL);
	i = -1;
	while (++i < s1_len)
		str[i] = s1[i];
	i = -1;
	while (s1_len + (++i) < s1_len + s2_len)
		str[s1_len + i] = s2[i];
	str[s1_len + s2_len] = '\0';
	return (str);
}
