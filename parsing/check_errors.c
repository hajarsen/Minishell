/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jari <marvin@42.ma>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 01:33:41 by sel-jari          #+#    #+#             */
/*   Updated: 2025/08/13 01:33:58 by sel-jari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static int	error_message(int i)
{
	if (i == 1)
	{
		printf("Minishell: syntax error near unexpectedtoken \'newline\'\n");
		return (1);
	}
	if (i == 2)
	{
		printf("Minishell: syntax error near unexpected token `||'\n");
		return (1);
	}
	if (i == 3)
	{
		printf("Minishell: syntax error near unexpected token `>'\n");
		return (1);
	}
	else
		return (0);
}

int	check_parsing_errors(t_tokenizer *token)
{
	if (token->op != NOT_OP && token->op != LESS_LESS)
		return (error_message(1));
	while (token != NULL)
	{
		if (token->op != NOT_OP)
		{
			if (token->next == NULL)
				return (error_message(1));
			if (token->next->op != NOT_OP)
			{
				if (token->op == PIPE && token->next->op == PIPE)
				{
					return (error_message(2));
				}
				if (token->op != PIPE && token->next->op != PIPE)
				{
					return (error_message(3));
				}
			}
		}
		token = token->next;
	}
	return (0);
}
