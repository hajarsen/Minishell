/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 05:16:21 by hsennane          #+#    #+#             */
/*   Updated: 2025/08/12 05:16:22 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	read_and_write_heredoc(t_tokenizer *token, t_env *env, int write_fd)
{
	char	*line;
	int		status;

	line = NULL;
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		status = process_heredoc_line(token, env, write_fd, line);
		if (status == 1)
			return (0);
		if (status == -1)
			return (-1);
	}
	if (!line)
	{
		ft_putstr_fd("minishell: warning: here-document\n", 2);
		ft_putstr_fd("delimited by end-of-file (wanted `", 2);
		ft_putstr_fd(token->str, 2);
		ft_putstr_fd("')\n", 2);
	}
	return (0);
}

static int	handle_fork_and_child(t_tokenizer *token, t_env *env, int *pipefd)
{
	int	ret;

	setup_heredoc_signals();
	close(pipefd[0]);
	ret = read_and_write_heredoc(token, env, pipefd[1]);
	close(pipefd[1]);
	if (ret == -1)
		_exit(1);
	_exit(0);
}

static int	handle_parent_process(pid_t pid, int *pipefd)
{
	int	status;

	close(pipefd[1]);
	(void)waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		glb_list()->exit_status = 130;
		close(pipefd[0]);
		return (-1);
	}
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		close(pipefd[0]);
		return (-1);
	}
	return (pipefd[0]);
}

int	open_heredoc_and_write_pipe(t_tokenizer *token, t_env *env,
		int *exit_status)
{
	int		pipefd[2];
	pid_t	pid;

	(void)exit_status;
	if (pipe(pipefd) == -1)
	{
		ft_putstr_fd("minishell: pipe error\n", 2);
		return (-1);
	}
	glb_list()->termination_status = 0;
	pid = fork();
	if (pid < 0)
	{
		ft_putstr_fd("minishell: fork error\n", 2);
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	if (pid == 0)
		handle_fork_and_child(token, env, pipefd);
	return (handle_parent_process(pid, pipefd));
}

void	process_heredocs(t_tokenizer *tokens, t_env *env, int *exit_status)
{
	t_tokenizer	*temp;

	temp = tokens;
	while (temp)
	{
		if (temp->op == LESS_LESS)
		{
			temp = temp->next;
			if (temp)
				temp->redirect.file_fd = open_heredoc_and_write_pipe(temp, env,
						exit_status);
		}
		if (temp)
			temp = temp->next;
	}
}
