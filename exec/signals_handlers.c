#include "minishell.h"

void	signal_handler_general(int signum)
{
	if (signum == SIGINT)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		/* Safe to use readline helpers only at the interactive prompt */
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
	else if (signum == SIGQUIT)
	{
		/* Ignore visual noise while parent waits */
		/* Do not print "Quit (core dumped)" in the shell itself */
	}
}

static void	handle_heredoc_sigint(void)
{
    /* Parent side during heredoc: print newline and mark status only */
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
