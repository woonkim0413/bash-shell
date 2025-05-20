/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_setting_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:09:15 by rakim             #+#    #+#             */
/*   Updated: 2025/05/20 15:44:55 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_parent(const char *p);
static void	get_parent_extend(int last, char *parent, const char *p);

int	validate_redirect(const char *p, int type)
{
	struct stat	st;
	struct stat	pst;
	char		*parent;
	int			result;

	result = 0;
	parent = get_parent(p);
	if (!parent)
		result = 1;
	else if (type == TOKEN_REDIR_IN)
	{
		if (stat(p, &st) < 0 || !S_ISREG(st.st_mode))
			result = 1;
	}
	else
	{
		if ((stat(p, &st) == 0 && S_ISDIR(st.st_mode)) || \
		(stat(parent, &pst) < 0 || !S_ISDIR(pst.st_mode)))
			result = 1;
	}
	free(parent);
	return (result);
}

static char	*get_parent(const char *p)
{
	int		last;
	char	*parent;
	int		i;

	last = -1;
	i = 0;
	while (p[i])
	{
		if (p[i] == '/')
			last = i;
		i++;
	}
	if (last >= 0)
		parent = ft_calloc(last + 1, 1);
	else
		parent = ft_calloc(2, 1);
	if (!parent)
		return (NULL);
	get_parent_extend(last, parent, p);
	return (parent);
}

static	void	get_parent_extend(int last, char *parent, const char *p)
{
	if (last >= 0)
	{
		ft_memcpy(parent, p, last);
		parent[last] = '\0';
	}
	else
	{
		parent[0] = '.';
		parent[1] = '\0';
	}
}
