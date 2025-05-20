/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <rakim@student.42gyeongsan.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 16:14:24 by rakim             #+#    #+#             */
/*   Updated: 2024/10/21 08:28:06 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static	int	get_digit(int n)
{
	int	result;

	if (n < 0)
		n = -n;
	result = 1;
	while (n / 10 > 0)
	{
		result++;
		n /= 10;
	}
	return (result);
}

static	void	put_num(char *result, int n, int end)
{
	while (n / 10 > 0)
	{
		result[end--] = n % 10 + '0';
		n /= 10;
	}
	result[end] = n + '0';
}

static	int	check_exception(int n, char **result)
{
	if (n == 0)
	{
		*result = ft_strdup("0");
		return (1);
	}
	else if (n == -2147483648)
	{
		*result = ft_strdup("-2147483648");
		return (1);
	}
	return (0);
}

char	*ft_itoa(int n)
{
	char	*result;
	int		total_len;

	if (check_exception(n, &result))
		return (result);
	total_len = get_digit(n);
	if (n < 0)
	{
		result = (char *)ft_calloc(total_len + 2, sizeof(char));
		if (result == NULL)
			return (NULL);
		result[0] = '-';
		result[total_len + 1] = '\0';
		if (n != -2147483648)
			put_num(result, -n, total_len);
	}
	else
	{
		result = (char *)ft_calloc(total_len + 1, sizeof(char));
		if (result == NULL)
			return (NULL);
		result[total_len] = '\0';
		put_num(result, n, total_len - 1);
	}
	return (result);
}
