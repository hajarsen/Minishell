#include "minishell.h"

static int open_redir_fd(t_tokenizer *op_tok)
{
    int fd = -1;

    if (!op_tok || !op_tok->next)
        return -1;
    if (op_tok->op == GREAT)
        fd = open(op_tok->next->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    else if (op_tok->op == GREAT_GREAT)
        fd = open(op_tok->next->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else if (op_tok->op == LESS)
        fd = open(op_tok->next->str, O_RDONLY);
    else if (op_tok->op == LESS_LESS)
    {
        if (op_tok->next->redirect.file_fd >= 0)
            fd = op_tok->next->redirect.file_fd;
        else
            fd = open_heredoc_and_write_pipe(op_tok->next, glb_list()->env, NULL);
    }
    return fd;
}

int execute_redirections(t_tokenizer *tokens)
{
    while (tokens)
    {
        if ((tokens->op == GREAT || tokens->op == GREAT_GREAT) && tokens->next)
        {
            int fd = tokens->next->redirect.file_fd;
            if (fd < 0)
                fd = open_redir_fd(tokens);
            if (fd < 0)
            {
                ft_putstr_fd("minishell: ", 2);
                ft_putstr_fd(tokens->next->str, 2);
                ft_putstr_fd(": ", 2);
                ft_putstr_fd(strerror(errno), 2);
                ft_putchar_fd('\n', 2);
                return 1;
            }
            if (dup2(fd, STDOUT_FILENO) < 0)
            {
                perror("dup2");
                if (fd > 2)
                    close(fd);
                return 1;
            }
            if (fd > 2)
                close(fd);
        }
        else if ((tokens->op == LESS || tokens->op == LESS_LESS) && tokens->next)
        {
            int fd = tokens->next->redirect.file_fd;
            if (fd < 0)
                fd = open_redir_fd(tokens);
            if (fd < 0)
            {
                ft_putstr_fd("minishell: ", 2);
                ft_putstr_fd(tokens->next->str, 2);
                ft_putstr_fd(": ", 2);
                ft_putstr_fd(strerror(errno), 2);
                ft_putchar_fd('\n', 2);
                return 1;
            }
            if (dup2(fd, STDIN_FILENO) < 0)
            {
                perror("dup2");
                if (fd > 2)
                    close(fd);
                return 1;
            }
            if (fd > 2)
                close(fd);
        }
        tokens = tokens->next;
    }
    return 0;
}
