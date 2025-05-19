/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woonkim <woonkim@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 12:32:46 by woonkim           #+#    #+#             */
/*   Updated: 2025/05/19 15:26:01 by woonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exit_extend(t_object *object, t_imp_stus *imp_stus, char **argv);
static int	is_valid_numeric(const char *str, long long *num);
static int	is_digit_char(char c);

int	execute_exit(t_object *object, t_imp_stus *imp_stus)
{
	char	**argv;

	argv = object->cmd_info->evecve_argv;
	if (argv[1])
	{
		if (argv[2])
		{
			write(imp_stus->stdout_fd, "bash: exit: too many arguments\n", 32);
			object->last_exit_status = 1;
			return (1);
		}
		exit_extend(object, imp_stus, argv);
	}
	write(imp_stus->stdout_fd, "exit\n", 5);
	free_stus_and_object(object, imp_stus);
	exit(0);
}

static void	exit_extend(t_object *object, t_imp_stus *imp_stus, char **argv)
{
	char		*temp;
	char		*error_message;
	long long	num;

	num = 0;
	if (is_valid_numeric(argv[1], &num))
	{
		write(imp_stus->stdout_fd, "exit\n", 5);
		free_stus_and_object(object, imp_stus);
		object->last_exit_status = 0;
		exit((unsigned char)num);
	}
	temp = ft_strjoin("bash: exit: ", argv[1]);
	error_message = ft_strjoin(temp, ": numeric argumnet required\n");
	free(temp);
	write(imp_stus->stdout_fd, error_message, ft_strlen(error_message));
	free(error_message);
	free_stus_and_object(object, imp_stus);
	exit(255);
}

static int	is_valid_numeric(const char *str, long long *num)
{
	int	idx;
	int	sign;

	sign = 1;
	idx = 0;
	if (str[idx] == '+' || str[idx] == '-')
	{
		if (str[idx] == '-')
			sign *= -1;
		idx++;
	}
	while (str[idx])
	{
		if (!is_digit_char(str[idx]))
			return (0);
		if (sign == 1 && *num > (LLONG_MAX - (str[idx] - '0')) / 10)
			return (0);
		if (sign == -1 && *num < (LLONG_MIN + (str[idx] - '0')) / 10)
			return (0);
		*num = (*num * 10) + str[idx] - '0';
		idx ++;
	}
	*num = *num * sign;
	return (1);
}

static int	is_digit_char(char c)
{
	return (c >= '0' && c <= '9');
}
