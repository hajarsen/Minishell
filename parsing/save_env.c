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

static int	set_env_node(t_env *new_node, char *env_line, int equal_pos)
{
	if (env_line[equal_pos] == '=')
	{
		new_node->name = ft_substr_(env_line, 0, equal_pos);
		new_node->value = ft_substr_(env_line, equal_pos + 1,
				ft_strlen(env_line));
	}
	else
	{
		new_node->name = ft_strdup_(env_line);
		new_node->value = ft_strdup_("");
	}
	if (!new_node->name || !new_node->value)
	{
		free(new_node->name);
		free(new_node->value);
		return (0);
	}
	new_node->next = NULL;
	return (1);
}

static void	add_node_to_list(t_env *new_node, t_env **head, t_env **tail)
{
	if (*head == NULL)
		*head = new_node;
	else
	{
		(*tail)->next = new_node;
	}
	*tail = new_node;
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

	if (!env)
		return (NULL);
	head = NULL;
	tail = NULL;
	i = 0;
	while (env[i])
	{
		new_node = malloc(sizeof(t_env));
		if (!new_node || !set_env_node(new_node, env[i],
				find_equal_position(env[i])))
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
