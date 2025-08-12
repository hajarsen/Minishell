/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 05:16:58 by hsennane          #+#    #+#             */
/*   Updated: 2025/08/12 05:17:02 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_redirection_token(t_tokenizer *tokens,
	t_tokenizer *start, t_env *env)
{
	int	fd;
	int	has_error;

	fd = open_redirection_file(tokens, env);
	if (fd < 0)
	{
		has_error = handle_redirection_error(tokens, start);
		if (has_error)
			return (has_error);
	}
	tokens->next->redirect.file_fd = fd;
	return (0);
}

int	redirection_infos(t_tokenizer *tokens)
{
	int			has_error;
	t_tokenizer	*start;
	t_env		*env;

	start = tokens;
	env = glb_list()->env;
	has_error = 0;
	while (tokens)
	{
		if ((tokens->op == GREAT || tokens->op == GREAT_GREAT
				|| tokens->op == LESS || tokens->op == LESS_LESS)
			&& tokens->next)
		{
			has_error = process_redirection_token(tokens, start, env);
			if (has_error)
				return (has_error);
		}
		tokens = tokens->next;
	}
	return (has_error);
}

void	close_redirection_fds(t_tokenizer *token)
{
	while (token)
	{
		if (token->redirect.file_fd > 2)
			close(token->redirect.file_fd);
		token = token->next;
	}
}

int	redirections_present(t_tokenizer *tokens)
{
	while (tokens)
	{
		if (tokens->op == GREAT || tokens->op == GREAT_GREAT
			|| tokens->op == LESS || tokens->op == LESS_LESS)
			return (1);
		tokens = tokens->next;
	}
	return (0);
}
