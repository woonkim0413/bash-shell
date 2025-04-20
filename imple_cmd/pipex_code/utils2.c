/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 23:58:55 by woonkim           #+#    #+#             */
/*   Updated: 2025/02/25 09:10:56 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**change_quates(char **argv, int i);

char	**fuck_case_check(char **argv)
{
	int i;

	// grep이면 확인
	if (ft_strncmp(argv[0], "grep", 4) == 0)
	{
		i = 0;
		while (argv[i])
		{
			// 만약 quates가 존재하면 문자열 복제
			if (ft_strncmp(argv[i], "\'\'", 3) == 0)
			{
				argv = change_quates(argv, i);
				return (argv);
			}
			i ++;
		}
	}
	return (argv);
}

char	**change_quates(char **argv, int i)
{
    free(argv[i]); 
    argv[i] = (char *)malloc(sizeof(char) * 1); 
    if (!argv[i])
        return (NULL); 
    argv[i][0] = '\0';
    return (argv);
}
