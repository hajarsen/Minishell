/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_setup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 05:17:15 by hsennane          #+#    #+#             */
/*   Updated: 2025/08/12 05:17:17 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	setup_signal_action(int signum, void (*handler)(int), int flags)
{
	struct sigaction	act;

	sigemptyset(&act.sa_mask);
	act.sa_handler = handler;
	act.sa_flags = flags;
	sigaction(signum, &act, NULL);
}

void	setup_signals(void)
{
	setup_signal_action(SIGINT, signal_handler_general, SA_RESTART);
	setup_signal_action(SIGQUIT, SIG_IGN, 0);
}

void	set_signal_handler(t_tokenizer *token)
{
	if (token && token->op == LESS_LESS)
	{
		setup_signal_action(SIGINT, signal_handler_heredoc, 0);
		setup_signal_action(SIGQUIT, signal_handler_heredoc, 0);
	}
}
