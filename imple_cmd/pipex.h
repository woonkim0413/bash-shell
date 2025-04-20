/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 15:36:10 by woonkim           #+#    #+#             */
/*   Updated: 2025/04/20 19:50:39 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/wait.h>

char		*find_path(char *command, char **main_envp);

void		memory_free(char** argv);

char	**fuck_case_check(char **argv);

char	*ft_strdup(const char *s);

char	**ft_split(char const *s, char c);

char	*ft_strjoin(char const *s1, char const *s2);

int	ft_strncmp(const char *s1, const char *s2, size_t n);

#endif