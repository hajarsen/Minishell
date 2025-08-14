/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_for_expanding.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jari <marvin@42.ma>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 01:35:03 by sel-jari          #+#    #+#             */
/*   Updated: 2025/08/13 01:35:06 by sel-jari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_tokenizer	*add_node(t_tokenizer **node)
{
	*node = gc_alloc(sizeof(t_tokenizer));
	if (*node == NULL)
	{
		exit(0);
	}
	(*node)->next = NULL;
	return (*node);
}

static int	alloc_quote(char *input, size_t *end)
{
	int		bl;
	char	c;

	bl = 0;
	c = input[*end];
	while (is_quote(c))
	{
		*end += 1;
		while (is_quote(input[*end]) != c)
			*end += 1;
		*end += 1;
		bl = 1;
		c = input[*end];
	}
	return (bl);
}

static int	alloc_str(char *input, int *i, t_tokenizer **node, int *node_i)
{
	size_t		end;
	char		*to_alloc;

	end = 0;
	while (input[*i + end] != '\0' && !ft_isspace(input[*i + end]))
	{
		if (alloc_quote(input + *i, &end) == 1)
			end--;
		end++;
	}
	if (end == 0)
		return (*i);
	*node = add_node(&((*node)->next));
	to_alloc = ft_substr(input, *i, end);
	fill_the_node_str(*node, *node_i, to_alloc);
	*i = *i + end;
	*node_i += 1;
	return (*i);
}

t_tokenizer	*tokenizer_for_expanding(char *input)
{
	t_tokenizer	*head;
	t_tokenizer	*node;
	int			i;
	int			node_i;

	i = 0;
	node_i = 0;
	head = add_node(&node);
	while (input[i])
	{
		if (input[i] == ' ')
		{
			i++;
			continue ;
		}
		i = alloc_str(input, &i, &node, &node_i);
		if (input[i] == 0)
			break ;
		i++;
	}
	node = head;
	head = head->next;
	free(node);
	return (head);
}

int	to_retokenize(t_tokenizer **token)
{
	int			i;
	char		c;

	i = 0;
	while ((*token)->str != NULL && (*token)->str[i] != 0)
	{
		c = (*token)->str[i];
		if (is_quote(c))
		{
			i++;
			while ((*token)->str[i] != 0 && (*token)->str[i] != c)
				i++;
		}
		if (isspace(c))
			return (1);
		i++;
	}
	return (0);
}
