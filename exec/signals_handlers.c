/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_handlers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 05:17:22 by hsennane          #+#    #+#             */
/*   Updated: 2025/08/12 05:17:24 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	signal_handler_general(int signum)
{
	if (signum == SIGINT)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		glb_list()->exit_status = 130;
	}
}

void	signal_handler_input(int signum)
{
	if (signum == SIGINT)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		glb_list()->exit_status = 130;
	}
}

static void	handle_heredoc_sigint(void)
{
	write(STDOUT_FILENO, "\n", 1);
	glb_list()->exit_status = 130;
}

static void	handle_heredoc_sigquit(void)
{
	ft_putstr_fd("\b\b  \b\b", STDOUT_FILENO);
}

void	signal_handler_heredoc(int signum)
{
	if (signum == SIGINT)
		handle_heredoc_sigint();
	else if (signum == SIGQUIT)
		handle_heredoc_sigquit();
}
