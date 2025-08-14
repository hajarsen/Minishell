/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_utils_extra.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jari <marvin@42.ma>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 01:34:10 by sel-jari          #+#    #+#             */
/*   Updated: 2025/08/13 01:34:12 by sel-jari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	*count_quotes(char *str)
{
	int		i;
	int		count;
	int		*quote_array;
	char	c;

	i = 0;
	count = 0;
	while (str[i])
	{
		c = str[i];
		if (is_quote(str[i]))
		{
			count++;
			i++;
			while (str[i] != c)
				i++;
		}
		i++;
	}
	if (count == 0)
		return (NULL);
	quote_array = gc_alloc(sizeof(int) * count * 2);
	return (quote_array);
}

int	valid_expanding(char *str, int *len)
{
	int			i;

	i = 1;
	if (str[1] == '?')
	{
		*len = 2;
		return (1);
	}
	if (!ft_isalnum(str[1]) && str[i] != '_')
		return (0);
	if (ft_isdigit(str[1]))
	{
		*len = 2;
		return (1);
	}
	while (ft_isalnum(str[i]) || str[i] == '_')
		i++;
	*len = i;
	return (1);
}

static void	expand_dq(t_tokenizer *token, int *i)
{
	char	*env_value;
	int		len;
	int		check;

	if ((token)->str[*i] == '$')
	{
		if (valid_expanding((token)->str + *i, &len))
		{
			env_value = check_env(ft_substr((token)->str, *i + 1, len - 1),
					&check);
			(token)->str = re_alloc((token)->str, i, len, env_value);
		}
	}
}

int	expand_nq(t_tokenizer **token, int *i)
{
	char	*env_value;
	int		len;
	int		check;

	check = 1;
	if ((*token)->str[*i] == '$')
	{
		if (remove_dollar_quote(token, i) == 0)
			return (0);
		if (valid_expanding((*token)->str + *i, &len))
		{
			env_value = check_env(ft_substr((*token)->str, *i + 1, len - 1),
					&check);
			if (check == 1 && env_value != NULL)
				env_value = modify_envar_for_nq(env_value);
			(*token)->str = re_alloc((*token)->str, i, len, env_value);
			free(env_value);
			return (1);
		}
	}
	return (0);
}

void	save_index(t_tokenizer *token)
{
	int		i;
	int		qi;
	char	c;

	i = 0;
	qi = 0;
	token->quotes_index = count_quotes(token->str);
	while (token->str[i] != 0 && token->quotes_index != NULL)
	{
		c = token->str[i];
		if (is_quote(c))
		{
			token->quotes_index[qi++] = i;
			i++;
			while (token->str[i] != c)
			{
				if (c == '\"')
					expand_dq(token, &i);
				i++;
			}
			token->quotes_index[qi++] = i;
		}
		i++;
	}
}
