/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirections.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 05:16:08 by hsennane          #+#    #+#             */
/*   Updated: 2025/08/12 05:38:53 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	open_redir_fd(t_tokenizer *op_tok)
{
	int	fd;

	fd = -1;
	if (!op_tok || !op_tok->next)
		return (-1);
	if (op_tok->op == GREAT)
		fd = open(op_tok->next->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (op_tok->op == GREAT_GREAT)
		fd = open(op_tok->next->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (op_tok->op == LESS)
		fd = open(op_tok->next->str, O_RDONLY);
	else if (op_tok->op == LESS_LESS)
	{
		if (op_tok->next->redirect.file_fd >= 0)
			fd = op_tok->next->redirect.file_fd;
		else
			fd = open_heredoc_and_write_pipe(op_tok->next, glb_list()->env,
					NULL);
	}
	return (fd);
}

static int	handle_fd(int fd, int std_fd, char *filename)
{
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(filename, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (1);
	}
	if (dup2(fd, std_fd) < 0)
	{
		ft_putstr_fd("minishell: Permission denied\n", 2);
		if (fd > 2)
			close(fd);
		return (1);
	}
	if (fd > 2)
		close(fd);
	return (0);
}

int	execute_redirections(t_tokenizer *tokens)
{
	int	fd;

	while (tokens)
	{
		if ((tokens->op == GREAT || tokens->op == GREAT_GREAT))
		{
			fd = tokens->next->redirect.file_fd;
			if (fd < 0)
				fd = open_redir_fd(tokens);
			if (handle_fd(fd, STDOUT_FILENO, tokens->next->str))
				return (1);
		}
		else if ((tokens->op == LESS || tokens->op == LESS_LESS))
		{
			fd = tokens->next->redirect.file_fd;
			if (fd < 0)
				fd = open_redir_fd(tokens);
			if (handle_fd(fd, STDIN_FILENO, tokens->next->str))
				return (1);
		}
		tokens = tokens->next;
	}
	return (0);
}
