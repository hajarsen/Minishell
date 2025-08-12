/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hajar <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 06:53:26 by hajar             #+#    #+#             */
/*   Updated: 2025/08/12 06:53:29 by hajar            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ignore_interactive_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
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

void	close_all_pipes(int (*pfds)[2], int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		close(pfds[i][READING_END]);
		close(pfds[i][WRITING_END]);
		i++;
	}
}
