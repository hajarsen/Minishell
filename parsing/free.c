#include "minishell.h"

void	free_tokens(char *input, t_tokenizer *tokens)
{
	t_tokenizer	*temp;
	void		*tmp;

	while (tokens != NULL)
	{
		if (tokens->op == LESS_LESS && tokens->hd)
		{
			t_here_doc *hd = tokens->hd;
			while (hd != NULL)
			{
				if (hd->str)
					free(hd->str);
				tmp = hd;
				hd = hd->next;
				free(tmp);
			}
			tokens->hd = NULL;
		}
		free(tokens->quotes_index);
		free(tokens->str);
		temp = tokens;
		tokens = tokens->next;
		free(temp);
	}
	free(input);
}

// void	free_env(t_env *env)
// {
// 	void	*tmp;

// 	while (env != NULL)
// 	{
// 		free(env->name);
// 		free(env->value);
// 		tmp = env;
// 		env = env->next;
// 		free(tmp);
// 	}
// }
