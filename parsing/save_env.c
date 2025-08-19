/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jari <marvin@42.ma>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 17:07:51 by sel-jari          #+#    #+#             */
/*   Updated: 2025/08/13 17:07:52 by sel-jari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


char	*ft_substr_(char const *s, unsigned int start, size_t len)
{
	char	*ptr;
	size_t	i;

	i = 0;
	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
		return (ft_calloc(1, sizeof(char)));
	if (ft_strlen(s) <= len - start)
		ptr = malloc(ft_strlen(s) - start + 1);
	else
		ptr = malloc(len + 1);
	if (!ptr)
		return (0);
	while (i < len && s[start + i])
	{
		ptr[i] = s[start + i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}

char	*ft_strdup_(const char *s)
{
	char	*d;
	size_t	sz;

	sz = ft_strlen(s) + 1;
	d = (char *)malloc(sz);
	if (d)
		ft_memcpy(d, s, sz);
	return (d);
}

static int	set_env_node(t_env *new_node, char *env_line, int equal_pos)
{
	if (env_line[equal_pos] == '=')
	{
		new_node->name = ft_substr_(env_line, 0, equal_pos);
		if (!new_node->name)
			return (0);
		new_node->value = ft_substr_(env_line, equal_pos + 1,
				ft_strlen(env_line));
		if (!new_node->value)
		{
			free(new_node->name);
			return (0);
		}
	}
	else
	{
		new_node->name = ft_strdup_(env_line);
		if (!new_node->name)
			return (0);
		new_node->value = ft_strdup_("");
		if (!new_node->value)
		{
			free(new_node->name);
			return (0);
		}
	}
	new_node->next = NULL;
	return (1);
}

static void	add_node_to_list(t_env *new_node, t_env **head, t_env **tail)
{
	if (*head == NULL)
	{
		*head = new_node;
		*tail = new_node;
	}
	else
	{
		(*tail)->next = new_node;
		*tail = new_node;
	}
}

static int	find_equal_position(char *env_line)
{
	int	pos;

	pos = 0;
	while (env_line[pos] && env_line[pos] != '=')
		pos++;
	return (pos);
}

t_env	*save_env(char **env)
{
	t_env	*new_node;
	t_env	*head;
	t_env	*tail;
	int		i;
	int		equal_pos;

	i = 0;
	head = NULL;
	tail = NULL;
	if (!env)
		return (NULL);
	while (env[i] != 0)
	{
		new_node = malloc(sizeof(t_env));
		if (!new_node)
		{
			free_env(head);
			return (NULL);
		}
		equal_pos = find_equal_position(env[i]);
		if (!set_env_node(new_node, env[i], equal_pos))
		{
			free(new_node);
			free_env(head);
			return (NULL);
		}
		add_node_to_list(new_node, &head, &tail);
		i++;
	}
	return (head);
}
