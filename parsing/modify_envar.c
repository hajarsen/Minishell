/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modify_envar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jari <marvin@42.ma>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 20:36:34 by sel-jari          #+#    #+#             */
/*   Updated: 2025/08/14 20:36:40 by sel-jari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	quote_quotes(char *result, char *str, int len)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	while (i < len)
	{
		if (str[i] == '\'')
		{
			result[j++] = '"';
			result[j++] = '\'';
			result[j++] = '"';
		}
		else if (str[i] == '"')
		{
			result[j++] = '\'';
			result[j++] = '"';
			result[j++] = '\'';
		}
		else
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
}

char	*modify_envar_for_nq(char *str)
{
	int		len;
	char	*result;
	int		new_len;

	len = ft_strlen(str);
	new_len = len * 3;
	result = malloc(new_len + 1);
	if (!result)
		return (NULL);
	quote_quotes(result, str, len);
	return (result);
}

int	remove_dollar_quote(t_tokenizer **token, int *i)
{
	int	j;

	if ((*token)->str[*i + 1] != 0 && is_quote((*token)->str[*i + 1]))
	{
		j = *i;
		while ((*token)->str[j])
		{
			(*token)->str[j] = (*token)->str[j + 1];
			j++;
		}
		*i -= 1;
		return (0);
	}
	return (1);
}

void	free_str_and_itoa(char *temp_itoi, char *str)
{
	free(str);
	free(temp_itoi);
}

void	if_expand_null(t_tokenizer **token, t_tokenizer *current)
{
	t_tokenizer	*to_remove;

	if (current != NULL && current->op == NOT_OP
		&& current->str != NULL && current->str[0] == '\0')
	{
		if (current->next != NULL)
		{
			to_remove = current;
			*token = current->next;
			free(to_remove->str);
			free(to_remove);
		}
		else
		{
			current->op = ITS_NULL_EXPAND;
		}
	}
}
