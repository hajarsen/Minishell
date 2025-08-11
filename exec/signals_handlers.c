#include "minishell.h"


void signal_handler_general(int signum)
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

void signal_handler_input(int signum)
{
    if (signum == SIGINT)
    {
        ft_putstr_fd("\n", STDOUT_FILENO);
        glb_list()->exit_status = 130;
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
    else if (signum == SIGQUIT)
    {
        ft_putstr_fd("Quit (core dumped)\n", STDOUT_FILENO);
        glb_list()->exit_status = 131;
    }
}

void signal_handler_heredoc(int signum)
{
    pid_t child_pid;
    int fd;

    if (signum == SIGINT)
    {
        write(STDOUT_FILENO, "\n", 1);
        fd = open(SHELL_CHILD_PID_FILE, O_RDONLY);
        if (fd < 0)
        {
            ft_putstr_fd("Error: Failed to open PID file\n", STDERR_FILENO);
            _exit(1);
        }
        if (read(fd, &child_pid, sizeof(pid_t)) < 0)
        {
            ft_putstr_fd("Error: Failed to read PID\n", STDERR_FILENO);
            close(fd);
            _exit(1);
        }
        close(fd);
        kill(child_pid, SIGKILL);
        glb_list()->exit_status = 130;
    }
    else if (signum == SIGQUIT)
    {
        ft_putstr_fd("\b\b  \b\b", STDOUT_FILENO);
    }
}
