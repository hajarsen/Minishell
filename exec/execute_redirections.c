#include "minishell.h"

int execute_redirections(t_tokenizer *tokens)
{
    int original_stdin = -1;
    int original_stdout = -1;
    
    while (tokens)
    {
        if ((tokens->op == GREAT || tokens->op == GREAT_GREAT) && tokens->next)
        {
            if (tokens->next->redirect.file_fd < 0)
            {
                ft_putstr_fd("minishell: ", 2);
                ft_putstr_fd(tokens->next->str, 2);
                ft_putstr_fd(": ", 2);
                ft_putstr_fd(strerror(tokens->next->redirect.errnum), 2);
                ft_putchar_fd('\n', 2);
                return 1;
            }
            if (original_stdout == -1)
                original_stdout = dup(STDOUT_FILENO);
            if (dup2(tokens->next->redirect.file_fd, STDOUT_FILENO) < 0)
            {
                perror("dup2");
                if (original_stdout != -1)
                    close(original_stdout);
                return 1;
            }
        }
        else if ((tokens->op == LESS || tokens->op == LESS_LESS) && tokens->next)
        {
            if (tokens->next->redirect.file_fd < 0)
            {
                ft_putstr_fd("minishell: ", 2);
                ft_putstr_fd(tokens->next->str, 2);
                ft_putstr_fd(": ", 2);
                ft_putstr_fd(strerror(tokens->next->redirect.errnum), 2);
                ft_putchar_fd('\n', 2);
                return 1;
            }
            if (original_stdin == -1)
                original_stdin = dup(STDIN_FILENO);
            if (dup2(tokens->next->redirect.file_fd, STDIN_FILENO) < 0)
            {
                perror("dup2");
                if (original_stdin != -1)
                    close(original_stdin);
                return 1;
            }
        }
        tokens = tokens->next;
    }
    return 0;
}
