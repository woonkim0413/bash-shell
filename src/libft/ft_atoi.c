/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 14:59:34 by rakim             #+#    #+#             */
/*   Updated: 2024/10/05 16:08:20 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	move_pointer(const char **nptr)
{
	int	result;

	result = 1;
	while (**nptr < 48 || **nptr > 57)
	{
		if (**nptr == ' ' || **nptr == '\t' \
		|| **nptr == '\n' || **nptr == '\v' \
		|| **nptr == '\f' || **nptr == '\r')
			(*nptr)++;
		else if (**nptr == '-')
		{
			result *= -1;
			(*nptr)++;
			break ;
		}
		else if (**nptr == '+')
		{
			(*nptr)++;
			break ;
		}
		else
			break ;
	}
	return (result);
}

int	ft_atoi(const char *nptr)
{
	int	sign;
	int	result;

	if (*nptr == '\0')
		return (0);
	sign = move_pointer(&nptr);
	result = 0;
	while (*nptr >= 48 && *nptr <= 57)
	{
		if (result == 0)
			result = *nptr - 48;
		else
		{
			result *= 10;
			result += *nptr - 48;
		}
		nptr++;
	}
	return (result * sign);
}
