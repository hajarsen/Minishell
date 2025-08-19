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
				ft_putstr_fd("minishell: Unexpected token\n", 2);
				return (1);
			}
		}
		i++;
	}
	return (0);
}

static int	only_spaces_tabs(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (ft_isspace(input[i]) == 0)
		{
			return (0);
		}
		i++;
	}
	return (1);
}

int	check_input_errors(char *input)
{
	if (!input)
	{
		printf("exit\n");
		rl_clear_history();
		gc_free_all();
		free_env(glb_list()->env);
		exit(glb_list()->exit_status);
		return (1);
	}
	if (ft_strlen(input) == 0)
	{
		return (1);
	}
	if (only_spaces_tabs(input))
		return (1);
	if (input_error(input) == 1)
	{
		glb_list()->exit_status = 2;
		add_history(input);
		return (1);
	}
	add_history(input);
	return (0);
}

static int	error_message(int i)
{
	if (i == 1)
	{
		gc_free_all();
		ft_putstr_fd("Minishell: syntax error near ", 2);
		ft_putstr_fd("unexpectedtoken \'newline\'\n", 2);
		glb_list()->exit_status = 2;
		return (1);
	}
	if (i == 2)
	{
		gc_free_all();
		ft_putstr_fd("Minishell: syntax error near unexpected token `|'\n", 2);
		glb_list()->exit_status = 2;
		return (1);
	}
	if (i == 3)
	{
		gc_free_all();
		ft_putstr_fd("Minishell: syntax error near unexpected token\n", 2);
		glb_list()->exit_status = 2;
		return (1);
	}
	else
		return (0);
}

int	check_parsing_errors(t_tokenizer *token)
{
	if (token->op == PIPE)
		return (error_message(2));
	if (token->next != NULL && token->op == LESS_LESS
		&& token->next->op == PIPE)
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
					return (error_message(2));
				if (token->op != PIPE && token->next->op != PIPE)
					return (error_message(3));
				if (token->op != PIPE && token->next->op == PIPE)
					return (error_message(2));
			}
		}
		token = token->next;
	}
	return (0);
}
