/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 20:10:44 by rakim             #+#    #+#             */
/*   Updated: 2025/05/12 17:25:11 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	int	is_file_path(char *src, t_check_redir_arg *arg)
{
	if (!(arg->redirect))
		return (0);
	else if (!(arg->redirect->file_path))
		return (0);
	else
	{
		if (!ft_strncmp(src, arg->redirect->file_path, \
		ft_strlen(arg->redirect->file_path)))
			return (1);
		return (0);
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
		line_splited_space = split_by_space_with_quote(line_splited_pipe[idx]);
		seperate_element(line_splited_space, current_node, object);
		idx++;
		if (line_splited_pipe[idx])
		{
			current_node->next = ft_calloc(sizeof(t_cmd_info), 1);
			current_node->next->prev = current_node;
			current_node = current_node->next;
		}
	}
	free_string_arr(&line_splited_pipe);
}
