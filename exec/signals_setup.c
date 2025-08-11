#include "minishell.h"

static void setup_signal_action(int signum, void (*handler)(int), int flags)
{
    struct sigaction act;

    sigemptyset(&act.sa_mask);
    act.sa_handler = handler;
    act.sa_flags = flags;
    sigaction(signum, &act, NULL);
}

void setup_signals(void)
{
    setup_signal_action(SIGINT, signal_handler_general, SA_RESTART);
    setup_signal_action(SIGQUIT, SIG_IGN, 0);
}

void set_signal_handler(t_tokenizer *token)
{
    if (token && token->op == LESS_LESS)
	{
        setup_signal_action(SIGINT, signal_handler_heredoc, SA_RESTART);
        setup_signal_action(SIGQUIT, signal_handler_heredoc, SA_RESTART);
    }
    else
    {
        setup_signal_action(SIGINT, signal_handler_input, SA_RESTART);
        setup_signal_action(SIGQUIT, signal_handler_input, SA_RESTART);
    }
}

