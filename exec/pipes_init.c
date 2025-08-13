/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 04:03:10 by hsennane          #+#    #+#             */
/*   Updated: 2025/08/13 04:03:13 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	alloc_pids(pid_t **pids, int nseg)
{
	*pids = (pid_t *)malloc(sizeof(pid_t) * nseg);
	if (!*pids)
	{
		ft_putendl_fd("minishell: malloc failed", 2);
		return (0);
	}
	return (1);
}

int	create_pipes(int nseg, int (**out_pfds)[2])
{
	int	i;
	int	(*pfds)[2];

	*out_pfds = NULL;
	if (nseg <= 1)
		return (0);
	pfds = (int (*)[2])malloc(sizeof (int [2]) * (nseg - 1));
	if (!pfds)
	{
		ft_putstr_fd("minishell: malloc failed\n", 2);
		return (-1);
	}
	i = -1;
	while (++i < nseg - 1)
	{
		if (pipe(pfds[i]) == -1)
		{
			cleanup_pipes(pfds, i);
			return (-1);
		}
	}
	*out_pfds = pfds;
	return (0);
}

int	init_segments(t_tokenizer ***starts, t_tokenizer ***ends,
		t_tokenizer *tokens, int nseg)
{
	*starts = gc_alloc(sizeof(t_tokenizer *) * nseg);
	*ends = gc_alloc(sizeof(t_tokenizer *) * nseg);
	if (!*starts || !*ends)
	{
		ft_putendl_fd("minishell: malloc failed", 2);
		return (0);
	}
	collect_segments(tokens, *starts, *ends, nseg);
	return (1);
}

int	init_pipeline_data(t_pipe_data *pipe_data, t_tokenizer *tokens)
{
	if (!init_segments(&pipe_data->starts, &pipe_data->ends, tokens,
			pipe_data->nseg))
		return (0);
	if (create_pipes(pipe_data->nseg, &pipe_data->pfds) < 0
		|| alloc_pids(&pipe_data->pids, pipe_data->nseg) < 0)
	{
		free_pipes(pipe_data->pfds, pipe_data->nseg - 1);
		return (0);
	}
	return (1);
}
