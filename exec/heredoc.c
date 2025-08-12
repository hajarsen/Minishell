#include "minishell.h"


int open_heredoc_and_write_pipe(t_tokenizer *token, t_env *env, int *exit_status)
{
    int     pipefd[2];
    pid_t   pid;
    int     status;

    (void)env;
    (void)exit_status;

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return -1;
    }

    glb_list()->termination_status = 0;

    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        close(pipefd[0]);
        close(pipefd[1]);
        return -1;
    }
    if (pid == 0)
    {
        struct sigaction act;
        sigemptyset(&act.sa_mask);
        act.sa_handler = SIG_DFL;
        act.sa_flags = 0;
        sigaction(SIGINT, &act, NULL);
        act.sa_handler = SIG_IGN;
        sigaction(SIGQUIT, &act, NULL);

        close(pipefd[0]); /* Close read end in child */

        while (1)
        {
            char *line = readline("> ");
            if (!line)
            {
                /* EOF (Ctrl-D): warn and finish heredoc */
                ft_putstr_fd("minishell: warning: here-document delimited by end-of-file (wanted `", 2);
                ft_putstr_fd(token->str, 2);
                ft_putstr_fd("')\n", 2);
                break;
            }
            if (ft_strcmp(line, token->str) == 0)
            {
                free(line);
                break;
            }
            if (write(pipefd[1], line, ft_strlen(line)) == -1
                || write(pipefd[1], "\n", 1) == -1)
            {
                perror("write");
                free(line);
                close(pipefd[1]);
                _exit(1);
            }
            free(line);
        }

        close(pipefd[1]);
        _exit(0);
    }

    close(pipefd[1]);

    (void)waitpid(pid, &status, 0);

    if (WIFSIGNALED(status))
    {
        int sig = WTERMSIG(status);
        if (sig == SIGINT)
        {
            glb_list()->exit_status = 130;
            close(pipefd[0]);
            errno = EINTR;
            return -1;
        }
    }
    else if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
    {
        close(pipefd[0]);
        return -1;
    }

    return pipefd[0];
}


void process_heredocs(t_tokenizer *tokens, t_env *env, int *exit_status) {
    while (tokens) {
        if (tokens->op == LESS_LESS) {
            tokens = tokens->next;
            if (tokens)
                tokens->redirect.file_fd = open_heredoc_and_write_pipe(tokens, env, exit_status);
        }
        if (tokens)
            tokens = tokens->next;
    }
}
