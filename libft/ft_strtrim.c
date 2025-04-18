/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 14:41:54 by rakim             #+#    #+#             */
/*   Updated: 2024/10/23 16:18:38 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static	int	get_last(char const *s1, char const *set, int end)
{
	int	set_idx;
	int	is_change;

	is_change = 0;
	set_idx = 0;
	while (end >= 0)
	{
		while (set[set_idx])
		{
			if (s1[end] == set[set_idx])
			{
				end--;
				is_change = 1;
				break ;
			}
			set_idx++;
		}
		if (is_change == 0)
			break ;
		is_change = 0;
		set_idx = 0;
	}
	return (end);
}

static	int	get_front(char const *s1, char const *set, int start)
{
	int	set_idx;
	int	is_change;

	set_idx = 0;
	is_change = 0;
	while (s1[start])
	{
		while (set[set_idx])
		{
			if (s1[start] == set[set_idx])
			{
				start++;
				is_change = 1;
				break ;
			}
			set_idx++;
		}
		if (is_change == 0)
			break ;
		is_change = 0;
		set_idx = 0;
	}
	return (start);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	s1_len;
	int		front;
	int		last;
	char	*result;
	int		result_idx;

	if ((!s1 && !set) || s1 == NULL)
		return (NULL);
	else if (set == NULL)
		return ((char *)s1);
	s1_len = ft_strlen(s1);
	result_idx = 0;
	front = get_front(s1, set, 0);
	last = get_last(s1, set, (int)s1_len - 1);
	if (last - front >= 0)
	{
		result = (char *)ft_calloc((last - front + 2), sizeof(char));
		if (result == 0)
			return (0);
		while (front <= last)
			result[result_idx++] = s1[front++];
	}
	else
		result = ft_strdup("");
	return (result);
}
