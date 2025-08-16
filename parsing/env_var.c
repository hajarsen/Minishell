/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jari <marvin@42.ma>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 02:31:56 by sel-jari          #+#    #+#             */
/*   Updated: 2025/08/13 02:31:59 by sel-jari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tokenizer	**env_var(t_tokenizer **token)
{
	int			i;
	char		c;

	i = 0;
	while ((*token)->str[i] != 0)
	{
		c = (*token)->str[i];
		if (is_quote((*token)->str[i]))
		{
			i++;
			while ((*token)->str[i] != c)
				i++;
		}
		expand_nq(token, &i);
		i++;
	}
	if (to_retokenize(token) == 1)
		tokenize_the_envar(token);
	save_index(*token);
	return (token);
}

static void	remove_q(t_tokenizer *token, int *i, int *j)
{
	char	q;
	int		start;

	q = is_quote(token->str[*i]);
	if (q != 0 && token->quotes_index != NULL
		&& *i == (token->quotes_index[*j] - *j))
	{
		start = *i;
		*i += 1;
		*j += 1;
		while (q != is_quote(token->str[*i])
			|| *i != (token->quotes_index[*j] - (*j - 1)))
			*i += 1;
		remove_quote(token->str, start, *i);
		*j += 1;
		*i -= 2;
	}
}

int	quote_handling(t_tokenizer *token)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (token->str[i] != 0)
	{
		remove_q(token, &i, &j);
		i++;
	}
	return (0);
}

void	dont_expand_herdoc(t_tokenizer *tokens)
{
	t_tokenizer	*temp;

	temp = tokens;
	while (temp != NULL)
	{
		if (temp != NULL && temp->op == LESS_LESS && temp->next != NULL)
			temp->next->red_case = DO_NOT_EXPAND;
		temp = temp->next;
	}
}
