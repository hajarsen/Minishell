/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 06:20:37 by hsennane          #+#    #+#             */
/*   Updated: 2025/08/12 06:20:40 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_redirect_fds(t_tokenizer *tokens)
{
	while (tokens)
	{
		tokens->redirect.file_fd = -1;
		tokens->redirect.errnum = 0;
		tokens->redirect.qt = NO_QUOTE;
		tokens = tokens->next;
	}
}

int	open_redirection_file(t_tokenizer *token, t_env *env)
{
	if (token->op == GREAT)
		return (open(token->next->str, O_WRONLY | O_CREAT | O_TRUNC, 0644));
	else if (token->op == GREAT_GREAT)
		return (open(token->next->str, O_WRONLY | O_CREAT | O_APPEND, 0644));
	else if (token->op == LESS)
		return (open(token->next->str, O_RDONLY));
	else if (token->op == LESS_LESS)
		return (open_heredoc_and_write_pipe(token->next, env, NULL));
	return (-1);
}

int	handle_redirection_error(t_tokenizer *token, t_tokenizer *start)
{
	if (token->op != LESS_LESS)
	{
		close_redirection_fds(start);
		return (1);
	}
	return (0);
}
