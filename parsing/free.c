/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jari <marvin@42.ma>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 01:34:13 by sel-jari          #+#    #+#             */
/*   Updated: 2025/08/13 01:34:15 by sel-jari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_here_doc(t_here_doc *hd)
{
	t_here_doc	*temp;

	while (hd)
	{
		temp = hd;
		hd = hd->next;
		free(temp->str);
		free(temp);
	}
}

void	free_tokens(char *input, t_tokenizer *tokens)
{
	t_tokenizer	*temp;

	while (tokens)
	{
		temp = tokens;
		tokens = tokens->next;
		free(temp->str);
		temp->str = NULL;
		free(temp->quotes_index);
		temp->quotes_index = NULL;
		free_here_doc(temp->hd);
		temp->hd = NULL;
		free(temp);
		temp = NULL;
	}
	free(input);
}
