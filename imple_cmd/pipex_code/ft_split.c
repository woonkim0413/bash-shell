/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 15:36:49 by woonkim           #+#    #+#             */
/*   Updated: 2025/02/25 09:21:54 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	count_num_dump(char *s, char c)
{
	int	num;

	num = 0;
	if (*s != '\0' && c == 0)
		return (2);
	else if (*s == '\0' && c == 0)
		return (1);
	while (*s == c)
		s ++;
	while (*s != 0)
	{
		while (*s != c && *s != 0)
			s ++;
		while (*s == c)
			s ++;
		num ++;
	}
	return (num + 1);
}

char	*insert_to_string2(char *s, int start_index, int end_index)
{
	char	*str;
	int		i;

	i = 0;
	if (end_index - start_index == 0)
		return (0);
	str = (char *)malloc(sizeof(char) * (end_index - start_index) + 1);
	if (str == 0)
		return (0);
	while (start_index + i < end_index)
	{
		str[i] = s[start_index + i];
		i ++;
	}
	str[i] = '\0';
	return (str);
}

void	free_malloc(char **str, int index)
{
	while (0 <= index)
	{
		free(str[index]);
		index --;
	}
}

int	insert_to_string(char **str, char *s, char c, int num_dump)
{
	int	i;
	int	j;
	int	index;

	index = 0;
	i = 0;
	while (index < num_dump - 1)
	{
		while (s[i] && s[i] == c)
			i ++;
		j = i;
		while (s[i] != c && s[i] != 0)
			i ++;
		str[index] = insert_to_string2(s, j, i);
		if (str[index] == NULL)
		{
			free_malloc(str, index);
			return (0);
		}
		index ++;
	}
	str[index] = NULL;
	return (1);
}

char	**ft_split(char const *s, char c)
{
	int			num_dump;
	char		**str;
	int			check;

	if (s == 0)
		return (NULL);
	num_dump = count_num_dump((char *)s, c);
	str = (char **)malloc(sizeof(char *) * num_dump);
	if (str == NULL)
		return (NULL);
	if (num_dump == 1)
		str[0] = NULL;
	else
	{
		check = insert_to_string(str, (char *)s, c, num_dump);
		if (check == 0)
		{
			free(str);
			str = NULL;
		}
	}
	return (str);
}
