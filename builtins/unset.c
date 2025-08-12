/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 04:36:45 by hsennane          #+#    #+#             */
/*   Updated: 2025/08/12 04:36:47 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	delete_env_var(t_env **env_list, const char *name)
{
	t_env	*prev;
	t_env	*curr;

	curr = *env_list;
	prev = NULL;
	while (curr)
	{
		if (ft_strncmp(curr->name, name, ft_strlen(name) + 1) == 0)
		{
			if (prev)
				prev->next = curr->next;
			else
				*env_list = curr->next;
			free(curr->name);
			free(curr->value);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

void	builtin_unset(char **args, t_env **env_list)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (args[i][0] != '\0')
			delete_env_var(env_list, args[i]);
		i++;
	}
}
