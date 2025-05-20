/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_log.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 14:53:35 by woonkim           #+#    #+#             */
/*   Updated: 2025/05/19 13:34:03 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"
#include <stdarg.h>

#define LOG_LEVEL_DEBUG 0
#define LOG_LEVEL_INFO  1
#define BUF_SIZE 1024

static int		get_log_level(t_env *env);
static void		buf_put_str(const char *s, char *buf, int *pos);
static void		buf_put_int(long long num, char *buf, int *pos);
static void		handler(va_list args, char spec, char *buf, int *pos);

void	print_log(int stdout_fd, t_object *object, const char *format, ...)
{
	char	buf[BUF_SIZE];
	int		pos;
	va_list	args;

	pos = 0;
	if (get_log_level(object->env) != LOG_LEVEL_DEBUG)
		return ;
	buf_put_str("[DEBUG] ", buf, &pos);
	va_start(args, format);
	while (*format && pos < BUF_SIZE - 1)
	{
		if (*format == '%' && *(format + 1))
		{
			++format;
			handler(args, *format, buf, &pos);
		}
		else
			buf[pos++] = *format;
		++format;
	}
	va_end(args);
	write(stdout_fd, buf, pos);
}

static int	get_log_level(t_env *env)
{
	t_env	*temp;

	temp = env;
	while (temp)
	{
		if (!ft_strncmp(temp->key, "LOG", ft_strlen(temp->key)))
		{
			if (!ft_strncmp(temp->value, "DEBUG", 5))
				return (LOG_LEVEL_DEBUG);
			else if (!ft_strncmp(temp->value, "INFO", 4))
				return (LOG_LEVEL_INFO);
		}
		temp = temp->next;
	}
	return (LOG_LEVEL_INFO);
}

static void	handler(va_list args, char spec, char *buf, int *pos)
{
	if (spec == 'd')
		buf_put_int((long long)va_arg(args, int), buf, pos);
	else if (spec == 's')
		buf_put_str(va_arg(args, char *), buf, pos);
	else if (spec == 'u')
	{
		buf[(*pos)++] = '%';
		buf[(*pos)++] = spec;
	}
}

static void	buf_put_str(const char *s, char *buf, int *pos)
{
	if (!s)
		return ;
	while (*s && *pos < BUF_SIZE - 1)
		buf[(*pos)++] = *s++;
}

static void	buf_put_int(long long num, char *buf, int *pos)
{
	if (num < 0)
	{
		buf[(*pos)++] = '-';
		num = -num;
	}
	if (num >= 10)
		buf_put_int(num / 10, buf, pos);
	buf[(*pos)++] = '0' + (num % 10);
}
