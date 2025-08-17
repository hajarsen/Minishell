/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_cleanup_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jari <marvin@42.ma>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 12:35:37 by sel-jari          #+#    #+#             */
/*   Updated: 2025/08/17 12:35:39 by sel-jari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	wait_for_child(pid_t pid, int *last_status, int i, int nseg)
{
	int	status;
	int	sig;
	int	any_sigint;

	any_sigint = 0;
	if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WIFSIGNALED(status))
		{
			sig = WTERMSIG(status);
			if (sig == SIGINT)
				any_sigint = 1;
		}
		if (i == nseg - 1)
			*last_status = status;
	}
	return (any_sigint);
}

void	handle_signal_termination(int last_status, int any_sigint)
{
	int	sig;

	if (any_sigint)
		ft_putstr_fd("\n", STDOUT_FILENO);
	if (WIFSIGNALED(last_status))
	{
		sig = WTERMSIG(last_status);
		if (sig == SIGQUIT)
			ft_putstr_fd("Quit: (core dumped)\n", STDOUT_FILENO);
		glb_list()->exit_status = 128 + sig;
	}
	else if (any_sigint)
		glb_list()->exit_status = 130;
}
