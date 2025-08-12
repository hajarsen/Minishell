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
	/* Only specialize for heredoc; otherwise keep whatever is configured */
	if (token && token->op == LESS_LESS)
	{
		/* Parent is not in readline; use minimal handler */
		setup_signal_action(SIGINT, signal_handler_heredoc, 0);
		setup_signal_action(SIGQUIT, signal_handler_heredoc, 0);
	}
}

