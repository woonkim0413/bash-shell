/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 20:10:44 by rakim             #+#    #+#             */
/*   Updated: 2025/05/15 19:37:48 by rakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	void	set_next_node(t_cmd_info **current_node)
{
	(*current_node)->next = ft_calloc(sizeof(t_cmd_info), 1);
	(*current_node)->next->prev = (*current_node);
	(*current_node) = (*current_node)->next;
}

static	void	set_file_path_flag_and_cmd(t_cmd_info *node, char **src, \
	t_check_redir_arg *arg)
{
	if (!(node->cmd) && \
	(*(src[(*arg).current_src]) != '>' && *(src[(*arg).current_src]) != '<'))
	{
		if ((*arg).file_path_flag == 1)
			(*arg).file_path_flag = 0;
		else
		{
			node->cmd = ft_strdup(src[(*arg).current_src]);
			(*arg).file_path_flag = 0;
		}
	}
}

static	void	seperate_element(char **src, t_cmd_info *node, \
t_object *object)
{
	t_check_redir_arg	arg;

	arg.current_src = 0;
	arg.redirect = NULL;
	arg.file_path_flag = 0;
	while (src[arg.current_src])
	{
		arg.src = src;
		arg.object = object;
		arg.cmd = (node->cmd);
		check_redirect(&arg);
		if (!(arg.src) && !(arg.cmd) && !(arg.redirect))
			return ;
		set_file_path_flag_and_cmd(node, src, &arg);
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

	if (!line_splited_pipe)
		return ;
	idx = 0;
	head = ft_calloc(sizeof(t_cmd_info), 1);
	if (!head)
		throw_error("malloc error", object, NULL, NULL);
	object->cmd_info = head;
	current_node = head;
	while (line_splited_pipe[idx])
	{
		line_splited_space = split_by_space_with_quote(line_splited_pipe[idx]);
		seperate_element(line_splited_space, current_node, object);
		if (!line_splited_pipe && !current_node)
			return ;
		idx++;
		if (line_splited_pipe[idx])
			set_next_node(&current_node);
	}
	free_string_arr(&line_splited_pipe);
}
