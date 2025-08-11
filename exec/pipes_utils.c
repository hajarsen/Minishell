#include "minishell.h"

void save_exit_status(t_glb *glb, int status_code)
{
    glb->exit_status = status_code;
}

int has_pipe(t_tokenizer *tokens)
{
    while (tokens)
    {
        if (tokens->op == PIPE)
            return 1;
        tokens = tokens->next;
    }
    return 0;
}
