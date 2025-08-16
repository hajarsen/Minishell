/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 04:03:03 by hsennane          #+#    #+#             */
/*   Updated: 2025/08/13 04:03:04 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	save_exit_status(t_glb *glb, int status_code)
{
	glb->exit_status = status_code;
}

int	count_segments(t_tokenizer *tokens)
{
	int			count;
	t_tokenizer	*t;

	count = 0;
	t = tokens;
	while (t)
	{
		count++;
		while (t && t->op != PIPE)
			t = t->next;
		if (t && t->op == PIPE)
			t = t->next;
	}
	return (count);
}

void	collect_segments(t_tokenizer *tokens, t_tokenizer **starts,
		t_tokenizer **ends, int n)
{
	int			i;
	t_tokenizer	*t;
	t_tokenizer	*prev;

	i = 0;
	t = tokens;
	while (t && i < n)
	{
		prev = NULL;
		starts[i] = t;
		while (t && t->op != PIPE)
		{
			prev = t;
			t = t->next;
		}
		ends[i] = prev;
		if (t && t->op == PIPE)
			t = t->next;
		i++;
	}
}

void	setup_child_pipes(int idx, int nseg, int (*pfds)[2])
{
	int	j;

	if (idx > 0)
	{
		if (dup2(pfds[idx - 1][READING_END], STDIN_FILENO) < 0)
		{
			ft_putendl_fd("minishell: Permission denied", 2);
			exit(1);
		}
	}
	if (idx < nseg - 1)
	{
		if (dup2(pfds[idx][WRITING_END], STDOUT_FILENO) < 0)
		{
			ft_putendl_fd("minishell: dup2 failed", 2);
			exit(1);
		}
	}
	j = 0;
	while (j < nseg - 1)
	{
		close(pfds[j][READING_END]);
		close(pfds[j][WRITING_END]);
		j++;
	}
}

void	extract_exit_status(int status, int *exit_status)
{
	if (WIFEXITED(status))
		*exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		*exit_status = 128 + WTERMSIG(status);
	else
		*exit_status = 1;
}
