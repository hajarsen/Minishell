#include "minishell.h"

void init_redirect_fds(t_tokenizer *tokens)
{
    while (tokens)
    {
        tokens->redirect.file_fd = -1;
        tokens->redirect.errnum = 0;
        tokens = tokens->next;
    }
}

int redirection_infos(t_tokenizer *tokens)
{
    t_tokenizer *start = tokens;
    int has_error = 0;
    
    while (tokens)
    {
        if ((tokens->op == GREAT || tokens->op == GREAT_GREAT || 
             tokens->op == LESS || tokens->op == LESS_LESS) && tokens->next)
        {
            int fd = -1;

            if (tokens->op == GREAT)
                fd = open(tokens->next->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            else if (tokens->op == GREAT_GREAT)
                fd = open(tokens->next->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
            else if (tokens->op == LESS)
                fd = open(tokens->next->str, O_RDONLY);
            else if (tokens->op == LESS_LESS)
                fd = open_heredoc_and_write_pipe(tokens->next, glb_list()->env, NULL);

            if (fd < 0)
            {
                tokens->next->redirect.errnum = errno;
                has_error = 1;
                
                /* Stop opening further files if any redirection has an error */
                if (tokens->op != LESS_LESS) /* Heredocs are handled separately */
                {
                    /* Close all previously opened file descriptors */
                    close_redirection_fds(start);
                    return 1;
                }
            }
                
            tokens->next->redirect.file_fd = fd;
        }
        tokens = tokens->next;
    }
    
    return has_error;
}



void	close_redirection_fds(t_tokenizer *token)
{
	while (token)
	{
		if (token->redirect.file_fd > 2)
			close(token->redirect.file_fd);
		token = token->next;
	}
}

int	here_doc_present(t_tokenizer *tokens)
{
	while (tokens)
	{
		if (tokens->op == LESS_LESS)
			return (1);
		tokens = tokens->next;
	}
	return (0);
}

int	redirections_present(t_tokenizer *tokens)
{
	while (tokens)
	{
		if (tokens->op == GREAT || tokens->op == GREAT_GREAT || 
			tokens->op == LESS || tokens->op == LESS_LESS)
			return (1);
		tokens = tokens->next;
	}
	return (0);
}

