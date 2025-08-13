/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jari <marvin@42.ma>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 01:35:07 by sel-jari          #+#    #+#             */
/*   Updated: 2025/08/13 01:35:08 by sel-jari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_operator(char *str)
{
	t_operator	op;

	if (ft_strncmp("<<", str, 2) == 0)
		op = LESS_LESS;
	else if (ft_strncmp(">>", str, 2) == 0)
		op = GREAT_GREAT;
	else if (ft_strncmp("|", str, 1) == 0)
		op = PIPE;
	else if (ft_strncmp("<", str, 1) == 0)
		op = LESS;
	else if (ft_strncmp(">", str, 1) == 0)
		op = GREAT;
	else if (ft_strncmp("\"", str, 1) == 0)
		op = DOUBLE_QUOTE;
	else if (ft_strncmp("\'", str, 1) == 0)
		op = QUOTE;
	else
		return (NOT_OP);
	return (op);
}

char	*alloc_quote_help(char *str, int *i)
{
	int		len;
	char	*alc_str;
	char	c;

	len = 1;
	c = *str;
	while (str[len] != c)
		len++;
	alc_str = gc_alloc(len + 2);
	ft_strlcpy(alc_str, str, len + 2);
	*i = *i + len;
	return (alc_str);
}

void	fill_the_node_op(t_tokenizer *node, int op, int i)
{
	node->i = i;
	node->op = op;
	node->str = NULL;
	node->next = NULL;
	node->quotes_index = NULL;
	node->hd = NULL;
}

void	fill_the_node_str(t_tokenizer *node, int i, char *token)
{
	node->i = i;
	node->op = NOT_OP;
	node->str = token;
	node->next = NULL;
	node->quotes_index = NULL;
	node->hd = NULL;
}

char	is_quote(char c)
{
	if (c == '\'')
		return (c);
	else if (c == '\"')
		return (c);
	else
		return (0);
}
