/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 20:10:44 by rakim             #+#    #+#             */
/*   Updated: 2025/04/27 17:49:56 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	void	set_redirect(t_token_type token_type, char *file_path, \
	t_redirect **redirect, t_object *object)
{
	t_redirect	*new_node;
	t_redirect	*temp;

	new_node = ft_calloc(sizeof(t_redirect), 1);
	if (!new_node)
		throw_error("malloc error", object);
	if (!file_path)
		throw_error("there is no file_path", object);
	new_node->type = token_type;
	new_node->file_path = ft_strdup(file_path);
	if (!(*redirect))
		*redirect = new_node;
	else
	{
		temp = *redirect;
		while (temp->next)
			temp = temp->next;
		temp->next = new_node;
	}
}

static	int	check_redirect(char **src, t_redirect **redirect, \
	t_object *object, int out_idx, char **cmd)
{
	int		in_single;
	int		in_double;
	int		idx;

	idx = 0;
	in_single = 0;
	in_double = 0;
	while (src[out_idx][idx])
	{
		set_toggle(src[out_idx][idx], &in_single, &in_double);
		if (!in_single && !in_double)
		{
			if (src[out_idx][idx] == '<' && src[out_idx][idx + 1] == '<')
			{
				if (!(*cmd) && idx > 0)
					*cmd = ft_substr(src[out_idx], 0, idx);
				if (src[out_idx][idx + 2])
				{
					set_redirect(TOKEN_HEREDOC, src[out_idx] + (idx + 2), redirect, object);
					return (1);
				}
				else
				{
					set_redirect(TOKEN_HEREDOC, src[out_idx + 1], redirect, object);
					return (2);
				}
			}
			else if (src[out_idx][idx] == '>' && src[out_idx][idx + 1] == '>')
			{
				if (!(*cmd) && idx > 0)
					*cmd = ft_substr(src[out_idx], 0, idx);
				if (src[out_idx][idx + 2])
				{
					set_redirect(TOKEN_APPEND, src[out_idx] + (idx + 2), redirect, object);
					return (1);
				}
				else
				{
					set_redirect(TOKEN_APPEND, src[out_idx + 1], redirect, object);
					return (2);
				}
			}
			else if (src[out_idx][idx] == '>')
			{
				if (!(*cmd) && idx > 0)
					*cmd = ft_substr(src[out_idx], 0, idx);
				if (src[out_idx][idx + 1])
				{
					set_redirect(TOKEN_REDIR_OUT, src[out_idx] + (idx + 1), redirect, object);
					return (1);
				}
				else
				{
					set_redirect(TOKEN_REDIR_OUT, src[out_idx + 1], redirect, object);
					return (2);
				}

			}
			else if (src[out_idx][idx] == '<')
			{
				if (!(*cmd) && idx > 0)
					*cmd = ft_substr(src[out_idx], 0, idx);
				if (src[out_idx][idx + 1])
				{
					set_redirect(TOKEN_REDIR_IN, src[out_idx] + (idx + 1), redirect, object);
					return (1);
				}
				else
				{
					set_redirect(TOKEN_REDIR_IN, src[out_idx + 1], redirect, object);
					return (2);
				}
			}
		}
		idx++;
	}
	return (0);
}

static	void	seperate_element(char **src, t_cmd_info *node, \
t_object *object)
{
	int					out_idx;
	t_redirect			*redirect;
	int					is_file_path;

	redirect = NULL;
	out_idx = 0;
	is_file_path = 0;
	while (src[out_idx])
	{
		is_file_path = check_redirect(src, &redirect, object, out_idx, &(node->cmd));
		if (is_file_path == 2)
			out_idx++;
		if (!is_file_path && !(node->cmd))
			node->cmd = ft_strdup(src[out_idx]);
		if (!src[out_idx])
			break ;
		out_idx++;
	}
	node->evecve_argv = src;
	node->redirect = redirect;
}

void	parsing(char **line_splited_pipe, t_object *object)
{
	int			idx;
	t_cmd_info	*head;
	t_cmd_info	*current_node;
	char		**line_splited_space;

	idx = 0;
	head = ft_calloc(sizeof(t_cmd_info), 1);
	if (!head)
		throw_error("malloc error", object);
	object->cmd_info = head;
	current_node = head;
	while (line_splited_pipe[idx])
	{
		line_splited_space = split_with_quote(line_splited_pipe[idx]);
		seperate_element(line_splited_space, current_node, object);
		idx++;
		if (line_splited_pipe[idx])
		{
			current_node->next = ft_calloc(sizeof(t_cmd_info), 1);
			current_node->next->prev = current_node;
			current_node = current_node->next;
		}
	}
	free_string_arr(line_splited_pipe);
	clean_up_quote(object->cmd_info);
}
