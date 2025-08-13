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

static void	set_env_node(t_env *new_node, char *env_line, int equal_pos)
{
	if (env_line[equal_pos] == '=')
	{
		new_node->name = ft_substr(env_line, 0, equal_pos);
		new_node->value = ft_substr(env_line, equal_pos + 1,
				ft_strlen(env_line));
	}
	else
	{
		new_node->name = ft_strdup(env_line);
		new_node->value = ft_strdup("");
	}
	new_node->next = NULL;
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
		new_node = gc_alloc(sizeof(t_env));
		if (!new_node)
			return (NULL);
		equal_pos = find_equal_position(env[i]);
		set_env_node(new_node, env[i], equal_pos);
		add_node_to_list(new_node, &head, &tail);
		i++;
	}
	return (head);
}
