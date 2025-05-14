/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_parsing_handler.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakim <fkrdbs234@naver.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 13:53:11 by rakim             #+#    #+#             */
/*   Updated: 2025/05/13 21:08:01 by rakim            ###   ########.fr       */
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
		throw_error("malloc error", object, NULL, NULL);
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
	if (!(arg->src[arg->current_src + 1]))
	{
		throw_error("there is no file_path", arg->object, NULL, NULL);
		return ;
	}
	if (arg->src[arg->current_src][next_idx])
	{
		set_redirect(token, arg->src[arg->current_src] + next_idx, \
			&(arg->redirect), arg->object);
		*idx += next_idx - 1;
	}
	else
	{
		set_redirect(token, arg->src[arg->current_src + 1], \
			&(arg->redirect), arg->object);
		*idx += next_idx - 1;
	}
}

void	check_redirect(t_check_redir_arg *arg)
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
			if (!(arg->src) && !(arg->cmd) && !(arg->redirect))
				return ;
		}
	}
}
