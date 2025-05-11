/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_log.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 14:53:35 by woonkim           #+#    #+#             */
/*   Updated: 2025/05/11 19:06:57 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <stdarg.h>
#define LOG_LEVEL_DEBUG 0
#define LOG_LEVEL_INFO  1

static int get_log_level(t_env *env);

void print_log(int stdout_fd, t_object *object, const char *format, ...)
{
	int			log_level;
	va_list		args;

	log_level = get_log_level(object->env);

	if (log_level <= LOG_LEVEL_DEBUG)
	{
		va_start(args, format);
		dprintf(stdout_fd, "[DEBUG] ");
		// fromat과 가변인자로 만든 va_list가 있을 때 출력을 도와주는 함수
		vdprintf(stdout_fd, format, args);
		va_end(args);
	}
}

static int get_log_level(t_env *env)
{
	t_env *temp = env;

	while (temp)
	{
		if (!ft_strncmp(temp->key, "LOG", ft_strlen(temp->key)))
		{
			if (!ft_strncmp(temp->value, "LOG_DEBUG", 9))
				return LOG_LEVEL_DEBUG;
			else if (!ft_strncmp(temp->value, "LOG_INFO", 8))
				return LOG_LEVEL_INFO;
		}
		temp = temp->next;
	}
	// 환경변수가 없으면 INFO 수준으로 간주 (출력 안 함)
	return LOG_LEVEL_INFO;
}
