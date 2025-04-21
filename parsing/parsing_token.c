/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 20:10:44 by rakim             #+#    #+#             */
/*   Updated: 2025/04/21 17:37:54 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	int	set_redirect(t_token_type token_type, char *file_path, \
	t_redirect **redirect, t_object *object)
{
	t_redirect	*new_node;
	t_redirect	*temp;

	new_node = ft_calloc(sizeof(t_redirect), 1);
	if (!new_node)
		throw_error("malloc error", object);
	new_node->type = token_type;
	new_node->file_path = ft_strdup(file_path);
	if (!(*redirect))
		*redirect = new_node;
	else
	{
		temp = *redirect;
		while (temp)
			temp = temp->next;
		temp->next = new_node;
	}
	return (1);
}

static	int	check_redirect(char **src, t_redirect **redirect, int idx, \
	t_object *object)
{
	if (ft_strchr(src[idx], '>'))
		return (set_redirect(TOKEN_REDIR_OUT, src[idx + 1], redirect, object));
	if (ft_strchr(src[idx], '<'))
		return (set_redirect(TOKEN_REDIR_IN, src[idx + 1], redirect, object));
	if (ft_strnstr(src[idx], "<<", ft_strlen(src[idx])))
		return (set_redirect(TOKEN_HEREDOC, src[idx + 1], redirect, object));
	if (ft_strnstr(src[idx], ">>", ft_strlen(src[idx])))
		return (set_redirect(TOKEN_APPEND, src[idx + 1], redirect, object));
	return (0);
}

static	void	seperate_element(char **src, t_cmd_info *node, \
t_object *object)
{
	int					idx;
	int					cmd_idx;
	t_redirect			*redirect;

	idx = 0;
	cmd_idx = 0;
	redirect = NULL;
	while (src[idx])
	{
		if (ft_strchr(src[idx], '$'))
			handle_env(src, idx, object->env);
		if (!check_redirect(src, &redirect, idx, object) && !(node->cmd))
			node->cmd = ft_strdup(src[idx]);
		idx++;
	}
	node->evecve_argv = src;
	node->redirect = redirect;
}

void	parsing(char **line_splited_pipe, t_object *object)
{
	int			idx;
	t_cmd_info	*head;
	t_cmd_info	*next_node;
	char		**line_splited_whitespace;

	idx = 0;
	head = ft_calloc(sizeof(t_cmd_info), 1);
	if (!head)
		throw_error("malloc error", object);
	object->cmd_info = head;
	next_node = head;
	while (line_splited_pipe[idx])
	{
		line_splited_whitespace = \
		ft_split_from_whitespace(line_splited_pipe[idx]);
		seperate_element(line_splited_whitespace, next_node, object);
		print_cmd(next_node);
		next_node->next = ft_calloc(sizeof(t_cmd_info), 1);
		next_node->next->prev = next_node;
		next_node = next_node->next;
		idx++;
	}
	free_string_arr(line_splited_pipe);
}
