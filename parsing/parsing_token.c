/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 20:10:44 by rakim             #+#    #+#             */
/*   Updated: 2025/05/10 15:34:54 by rakim            ###   ########.fr       */
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
		throw_error("malloc error", object, NULL);
	if (!file_path)
		throw_error("there is no file_path", object, NULL);
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

static	void	handle_redirect(t_check_redir_arg *arg, \
	int *idx, t_token_type token)
{
	int	next_idx;

	next_idx = *idx;
	if (token == TOKEN_REDIR_IN || token == TOKEN_REDIR_OUT)
		next_idx++;
	if (token == TOKEN_APPEND || token == TOKEN_HEREDOC)
		next_idx += 2;
	if (arg->src[arg->current_src][next_idx])
	{
		set_redirect(token, arg->src[arg->current_src] + (next_idx), \
		&(arg->redirect), arg->object);
		*idx += next_idx - 1;
	}
	else
	{
		set_redirect(token, arg->src[arg->current_src + 1], \
			&(arg->redirect), arg->object);
		*idx += next_idx - 1;
		return (1);
	}
}

static	void	check_redirect(t_check_redir_arg *arg)
{
	int		in_single;
	int		in_double;
	int		idx;

	idx = -1;
	in_single = 0;
	in_double = 0;
	while (arg->src[arg->current_src][++idx])
	{
		set_toggle(arg->src[arg->current_src][idx], &in_single, &in_double);
		if (!in_single && !in_double)
		{
			if (arg->src[arg->current_src][idx] == '<' && \
				arg->src[arg->current_src][idx + 1] == '<')
				handle_redirect(arg, &idx, TOKEN_HEREDOC);
			else if (arg->src[arg->current_src][idx] == '>' \
				&& arg->src[arg->current_src][idx + 1] == '>')
				handle_redirect(arg, &idx, TOKEN_APPEND);
			else if (arg->src[arg->current_src][idx] == '>')
				handle_redirect(arg, &idx, TOKEN_REDIR_OUT);
			else if (arg->src[arg->current_src][idx] == '<')
				handle_redirect(arg, &idx, TOKEN_REDIR_IN);
		}
	}
}

static	int	is_file_path(char *src, t_check_redir_arg *arg)
{
	if (!(arg->redirect))
		return (0);
	else if (!(arg->redirect->file_path))
		return (0);
	else
	{
		if (ft_strncmp(src, arg->redirect->file_path, \
		ft_strlen(arg->redirect->file_path)))
			return (1);
	}
}

static	void	seperate_element(char **src, t_cmd_info *node, \
t_object *object)
{
	t_check_redir_arg	arg;

	arg.current_src = 0;
	arg.redirect = NULL;
	while (src[arg.current_src])
	{
		arg.src = src;
		arg.object = object;
		arg.cmd = (node->cmd);
		check_redirect(&arg);
		if (!(node->cmd) && \
		(*(src[arg.current_src]) != '>' && *(src[arg.current_src]) != '<') && \
		!is_file_path(src[arg.current_src], &arg))
			node->cmd = ft_strdup(src[arg.current_src]);
		if (!src[arg.current_src])
			break ;
		(arg.current_src)++;
	}
	node->evecve_argv = src;
	node->redirect = arg.redirect;
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
		throw_error("malloc error", object, NULL);
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
