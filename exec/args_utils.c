#include "minishell.h"

char **tokens_to_args(t_tokenizer *tokens)
{
    int count = 0;
    t_tokenizer *tmp = tokens;

    while (tmp)
    {
        if (tmp->op == LESS || tmp->op == LESS_LESS || 
            tmp->op == GREAT || tmp->op == GREAT_GREAT)
        {
            tmp = tmp->next;
            if (tmp)
                tmp = tmp->next;
        }
        else if (tmp->op == NOT_OP)
        {
            count++;
            tmp = tmp->next;
        }
        else
            tmp = tmp->next;
    }

    char **args = gc_alloc(sizeof(char *) * (count + 1));
    if (!args)
        return NULL;

    tmp = tokens;
    int i = 0;
    while (tmp)
    {
        if (tmp->op == LESS || tmp->op == LESS_LESS || 
            tmp->op == GREAT || tmp->op == GREAT_GREAT)
        {
            tmp = tmp->next;
            if (tmp)
                tmp = tmp->next;
        }
        else if (tmp->op == NOT_OP)
        {
            args[i++] = ft_strdup(tmp->str);
            tmp = tmp->next;
        }
        else
            tmp = tmp->next;
    }
    args[i] = NULL;
    return args;
}

void free_args(char **args)
{
    int i = 0;
    if (!args)
        return;
    while (args[i])
    {
        free(args[i]);
        i++;
    }
    free(args);
}
