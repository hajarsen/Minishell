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

int	check_input_errors(char *input)
{
	if (!input)
	{
		printf("exit\n");
		free_env(glb_list()->env);
		rl_clear_history();
		exit(glb_list()->exit_status);
	}
	if (ft_strlen(input) == 0)
	{
		free(input);
		return (1);
	}
	if (input_error(input) == 1)
	{
		free(input);
		return (1);
	}
	add_history(input);
	return (0);
}

static int	error_message(int i, char *input, t_tokenizer *token)
{
	if (i == 1)
	{
		free_tokens(input, token);
		printf("Minishell: syntax error near unexpectedtoken \'newline\'\n");
		glb_list()->exit_status = 2;
		return (1);
	}
	if (i == 2)
	{
		free_tokens(input, token);
		printf("Minishell: syntax error near unexpected token `||'\n");
		glb_list()->exit_status = 2;
		return (1);
	}
	if (i == 3)
	{
		free_tokens(input, token);
		printf("Minishell: syntax error near unexpected token\n");
		glb_list()->exit_status = 2;
		return (1);
	}
	else
		return (0);
}

int	check_parsing_errors(t_tokenizer *token, char *input)
{
	if (token->next != NULL && token->op == LESS_LESS
			&& token->next->op == PIPE)
		return (error_message(1, input, token));
	while (token != NULL)
	{
		if (token->op != NOT_OP)
		{
			if (token->next == NULL)
				return (error_message(1, input, token));
			if (token->next->op != NOT_OP)
			{
				if (token->op == PIPE && token->next->op == PIPE)
				{
					return (error_message(2, input, token));
				}
				if (token->op != PIPE && token->next->op != PIPE)
				{
					return (error_message(3, input, token));
				}
			}
		}
		token = token->next;
	}
	return (0);
}
