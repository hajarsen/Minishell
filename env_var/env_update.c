/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_update.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 05:10:10 by hsennane          #+#    #+#             */
/*   Updated: 2025/08/12 05:10:11 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	update_env_var(t_env *env_list, t_env *new_node)
{
	t_env	*tmp;
	char	*new_value;

	tmp = env_list;
	while (tmp)
	{
		if (ft_strcmp(tmp->name, new_node->name) == 0)
		{
			if (!new_node->value)
				return (1);
			new_value = ft_strdup(new_node->value);
			if (!new_value)
				return (1);
			free(tmp->value);
			tmp->value = new_value;
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

void	update_shell_lvl(t_env **env_list)
{
	char	*shell_lvl;
	int		lvl;
	char	*new_lvl_str;

	shell_lvl = get_env_value("SHLVL", *env_list);
	lvl = 1;
	if (shell_lvl)
		lvl = ft_atoi(shell_lvl) + 1;
	new_lvl_str = ft_itoa(lvl);
	if (!new_lvl_str)
		return ;
	insert_env_node(env_list, create_env_node(ft_strdup("SHLVL"), new_lvl_str));
}
