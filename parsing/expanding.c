/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jari <marvin@42.ma>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 02:46:13 by sel-jari          #+#    #+#             */
/*   Updated: 2025/08/13 02:46:14 by sel-jari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	str_has_quote(const char *s)
{
	int	i;

	i = 0;
	while (s && s[i])
	{
		if (s[i] == '\'' || s[i] == '"')
			return (1);
		i++;
	}
	return (0);
}

static int	prev_is_heredoc(t_tokenizer *head, t_tokenizer *node)
{
	t_tokenizer	*it;

	it = head;
	if (!head || !node)
		return (0);
	while (it && it->next)
	{
		if (it->next == node)
			return (it->op == LESS_LESS);
		it = it->next;
	}
	return (0);
}

void	remove_quote(char *str, int start, int end)
{
	int	i;

	i = 0;
	while (str[start + i + 1] != 0)
	{
		str[start + i] = str[start + i + 1];
		i++;
	}
	start = end - 1;
	i = 0;
	while (str[start + i + 1] != 0)
	{
		str[start + i] = str[start + i + 1];
		i++;
	}
	str[ft_strlen(str) - 2] = 0;
}

void	expanding(t_tokenizer **token)
{
	t_tokenizer	**temp;
	t_tokenizer	*to_remove;

	temp = token;
	while ((*temp) != NULL)
	{
		if ((*temp)->op == LESS_LESS)
			(*temp)->hd = NULL;
		if ((*temp)->op == NOT_OP)
		{
			if (prev_is_heredoc(*token, *temp))
				(*temp)->redirect.qt = str_has_quote((*temp)->str) ? THERES_QUOTE : NO_QUOTE;
			temp = env_var(temp);
		}
		if ((*temp)->op == NOT_OP)
			quote_handling((*temp));
		if ((*temp) != NULL && (*temp)->op == NOT_OP
			&& (*temp)->str != NULL && (*temp)->str[0] == '\0')
		{
			to_remove = *temp;
			*temp = (*temp)->next;
			free(to_remove->str);
			free(to_remove);
			continue ;
		}
		if ((*temp) == NULL)
			break ;
		temp = &(*temp)->next;
	}
}
