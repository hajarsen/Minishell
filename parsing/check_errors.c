#include "minishell.h"

int	input_error(char *input)
{
	int		i;
	int		error;
	char	c;

	i = 0;
	while (input[i])
	{
		error = 1;
		c = input[i];
		if (is_quote(c))
		{
			i++;
			while (input[i] && input[i] != c)
				i++;
			if (c == input[i])
				error = 0;
			if (error == 1)
			{
				printf("minishell: Unexpected token\n");
				return (1);
			}
		}
		i++;
	}
	return (0);
}

int	check_parsing_errors(t_tokenizer *token)
{
	while (token != NULL)
	{
		if (token->op != NOT_OP)
		{
			if (token->next == NULL)
			{
				printf("Minishell: syntax error near unexpected token `newline'\n");
				return (1);
			}
			if (token->next->op != NOT_OP)
			{
				if (token->op == PIPE && token->next->op == PIPE)
				{
					printf("Minishell: syntax error near unexpected token `||'\n");
					return (1);
				}
				if (token->op != PIPE && token->next->op != PIPE)
				{
					printf("Minishell: syntax error near unexpected token `>'\n");
					return (1);
				}
			}
		}
		token = token->next;
	}
	return (0);
}
