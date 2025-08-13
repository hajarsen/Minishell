#include "minishell.h"

t_tokenizer	**env_var(t_tokenizer **token)
{
	int			i;
	char		c;
	int			to_dele;

	i = 0;
	to_dele = 0;
	while ((*token)->str[i] != 0)
	{
		c = (*token)->str[i];
		if (is_quote((*token)->str[i]))
		{
			i++;
			while ((*token)->str[i] != c)
				i++;
		}
		to_dele = expand_nq(token, &i);
		i++;
	}
	if (to_retokenize(token) == 1)
		tokenize_the_envar(token);
	if (!to_dele)
		save_index(*token);
	return (token);
}
