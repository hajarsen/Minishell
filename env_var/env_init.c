/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 05:10:01 by hsennane          #+#    #+#             */
/*   Updated: 2025/08/12 05:10:02 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	extract_and_push(t_env **env_list, char *env_var)
{
	char	*name;
	char	*value;
	char	*equal;
	t_env	*new_node;

	equal = ft_strchr(env_var, '=');
	if (!equal)
	{
		name = ft_strdup(env_var);
		if (!name)
			return ;
		new_node = create_env_node(name, NULL);
	}
	else
	{
		name = ft_substr(env_var, 0, equal - env_var);
		value = ft_strdup(equal + 1);
		if (!name || (equal[1] && !value))
			return ;
		new_node = create_env_node(name, value);
	}
	if (new_node)
		insert_env_node(env_list, new_node);
}

void	init_env_var(t_env **env_list, char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		extract_and_push(env_list, envp[i]);
		i++;
	}
	update_shell_lvl(env_list);
}
