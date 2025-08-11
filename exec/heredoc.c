#include "minishell.h"


int open_heredoc_and_write_pipe(t_tokenizer *token, t_env *env, int *exit_status)
{
    int     pipefd[2];
    char    *line;

    (void)env;
    (void)exit_status;

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return -1;
    }

    while (1)
    {
        line = readline("> ");
        if (!line)
        {
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
        if (write(pipefd[1], line, ft_strlen(line)) == -1)
        {
            perror("write");
            free(line);
            close(pipefd[1]);
            close(pipefd[0]);
            return -1;
        }
        if (write(pipefd[1], "\n", 1) == -1)
        {
            perror("write");
            free(line);
            close(pipefd[1]);
            close(pipefd[0]);
            return -1;
        }
        free(line);
    }

    close(pipefd[1]);
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
