/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_cleanup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 04:02:55 by hsennane          #+#    #+#             */
/*   Updated: 2025/08/13 04:02:56 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	cleanup_pipes(int (*pfds)[2], int count)
{
	while (count-- > 0)
	{
		close(pfds[count][READING_END]);
		close(pfds[count][WRITING_END]);
	}
	free(pfds);
}

void	free_pipes(int (*pfds)[2], int n_pipes)
{
	if (!pfds)
		return ;
	close_all_pipes(pfds, n_pipes);
	free(pfds);
}

void	wait_children(pid_t *pids, int nseg, int *last_status)
{
	int	i;
	int	status;

	*last_status = 0;
	i = 0;
	while (i < nseg)
	{
		if (pids[i] > 0)
		{
			waitpid(pids[i], &status, 0);
			if (i == nseg - 1)
				*last_status = status;
		}
		i++;
	}
}

void	cleanup_pipeline(t_pipe_data *pipe_data, int *last_status)
{
	free_pipes(pipe_data->pfds, pipe_data->nseg - 1);
	wait_children(pipe_data->pids, pipe_data->nseg, last_status);
	free(pipe_data->pids);
}
