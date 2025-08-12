/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 05:10:43 by hsennane          #+#    #+#             */
/*   Updated: 2025/08/12 05:10:44 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*str_concat_three(const char *a, const char *b, const char *c)
{
	size_t	len_a;
	size_t	len_b;
	size_t	len_c;
	char	*joined;

	if (!a || !b || !c)
		return (NULL);
	len_a = ft_strlen(a);
	len_b = ft_strlen(b);
	len_c = ft_strlen(c);
	joined = malloc(len_a + len_b + len_c + 1);
	if (!joined)
		return (NULL);
	ft_memcpy(joined, a, len_a);
	ft_memcpy(joined + len_a, b, len_b);
	ft_memcpy(joined + len_a + len_b, c, len_c);
	joined[len_a + len_b + len_c] = '\0';
	return (joined);
}

static int	fill_env_array(char **array, t_env *env_list)
{
	int		idx;
	t_env	*cur;

	idx = 0;
	cur = env_list;
	while (cur)
	{
		if (cur->name)
		{
			if (cur->value)
				array[idx] = str_concat_three(cur->name, "=", cur->value);
			else
				array[idx] = ft_strdup(cur->name);
			if (!array[idx])
				return (-1);
			idx++;
		}
		cur = cur->next;
	}
	array[idx] = NULL;
	return (0);
}

char	**envlist_to_array(t_env *env_list)
{
	int		count;
	t_env	*tmp;
	char	**env_arr;

	count = 0;
	tmp = env_list;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	env_arr = gc_alloc(sizeof(char *) * (count + 1));
	if (!env_arr)
		return (NULL);
	if (fill_env_array(env_arr, env_list) == -1)
	{
		free(env_arr);
		return (NULL);
	}
	return (env_arr);
}
