/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 05:15:42 by hsennane          #+#    #+#             */
/*   Updated: 2025/08/12 05:15:43 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_args(t_tokenizer *t)
{
	int			count;
	t_tokenizer	*tmp;

	count = 0;
	tmp = t;
	while (tmp)
	{
		if (tmp->op == LESS || tmp->op == LESS_LESS
			|| tmp->op == GREAT || tmp->op == GREAT_GREAT)
		{
			tmp = tmp->next;
			if (tmp)
				tmp = tmp->next;
		}
		else if (tmp->op == NOT_OP)
		{
			count++;
			tmp = tmp->next;
		}
		else
			tmp = tmp->next;
	}
	return (count);
}

static void	fill_args(t_tokenizer *t, char **args)
{
	int			i;
	t_tokenizer	*tmp;

	i = 0;
	tmp = t;
	while (tmp)
	{
		if (tmp->op == LESS || tmp->op == LESS_LESS
			|| tmp->op == GREAT || tmp->op == GREAT_GREAT)
		{
			tmp = tmp->next;
			if (tmp)
				tmp = tmp->next;
		}
		else if (tmp->op == NOT_OP)
		{
			args[i] = ft_strdup(tmp->str);
			i++;
			tmp = tmp->next;
		}
		else
			tmp = tmp->next;
	}
	args[i] = NULL;
}

char	**tokens_to_args(t_tokenizer *tokens)
{
	int				count;
	char			**args;

	count = count_args(tokens);
	args = gc_alloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	fill_args(tokens, args);
	return (args);
}

void	free_args(char **args)
{
	int	i;

	i = 0;
	if (!args)
		return ;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}
