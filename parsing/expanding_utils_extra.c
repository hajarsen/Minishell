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
	// Handle special case $?
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

// static char	*word_splitting(char *env_value)
// {
// 	char	*fixed_env_value;
// 	int		i;
// 	int		j;

// 	fixed_env_value = malloc(ft_strlen(env_value));
// 	i = 0;
// 	j = 0;
// 	while (env_value[i])
// 	{
// 		while(ft_isspace(env_value[i]))
// 			i++;
// 		if (j != 0 && env_value[i] != 0)
// 			fixed_env_value[j++] = ' ';
// 		while (!ft_isspace(env_value[i]) && env_value[i] != 0)
// 		{
// 			fixed_env_value[j] = env_value[i];
// 			j++;
// 			i++;
// 		}
// 	}
// 	fixed_env_value[j] = 0;
// 	return (fixed_env_value);
// }

static void	expand_dq(t_tokenizer *token, int *i)
{
	char	*env_value;
	int		len;

	if ((token)->str[*i] == '$')
	{
		if (valid_expanding((token)->str + *i, &len))
		{
			env_value = check_env(ft_substr((token)->str, *i + 1, len - 1));
			(token)->str = re_alloc((token)->str, i, len, env_value);
		}
	}
}

int	expand_nq(t_tokenizer **token, int *i)
{
	char	*env_value;
	int		len;

	if ((*token)->str[*i] == '$')
	{
		if (valid_expanding((*token)->str + *i, &len))
		{
			env_value = check_env(ft_substr((*token)->str, *i + 1, len - 1));
			(*token)->str = re_alloc((*token)->str, i, len, env_value);
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
				{
					expand_dq(token, &i);
				}
				i++;
			}
			token->quotes_index[qi++] = i;
		}
		i++;
	}
	// printf("this is pointer token : %p and %ld and %s\n",token ,malloc_usable_size(token->quotes_index), (token)->str);
	// for(int k = 0 ; k < 4 && token->quotes_index; k++)
	// 	printf("token---->  %d\n", token->quotes_index[k]);
	// printf("here s : %s", token->str);// 
}
