/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 04:36:37 by hsennane          #+#    #+#             */
/*   Updated: 2025/08/12 04:36:39 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_identifier(const char *s)
{
	int	i;

	i = 0;
	if (!s || !*s)
		return (0);
	if (!(ft_isalpha(s[i]) || s[i] == '_'))
		return (0);
	if (s[i + 1] == '-' && (s[i + 2] == '=' || s[i + 2] == '\0'))
		return (0);
	while (s[i] && s[i] != '=')
	{
		if (!(ft_isalnum(s[i]) || s[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

static void	get_name_value(char *arg, char **name, char **value)
{
	char	*eq;

	eq = ft_strchr(arg, '=');
	if (eq)
	{
		*name = ft_substr(arg, 0, eq - arg);
		*value = ft_strdup(eq + 1);
	}
	else
	{
		*name = ft_strdup(arg);
		*value = NULL;
	}
}

static void	update_or_add_export(char *arg, t_env **env_list)
{
	char	*name;
	char	*value;
	t_env	*new_node;

	name = NULL;
	value = NULL;
	get_name_value(arg, &name, &value);
	if (name)
	{
		new_node = create_env_node(name, value);
		if (update_env_var(*env_list, new_node))
		{
			free(new_node->name);
			free(new_node->value);
			free(new_node);
		}
		else
			insert_env_node(env_list, new_node);
	}
}

static int	process_export_args(char **args, t_env **env_list)
{
	int	i;
	int	ret_val;

	i = 1;
	ret_val = 0;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			ret_val = 1;
		}
		else
			update_or_add_export(args[i], env_list);
		i++;
	}
	return (ret_val);
}

int	ft_export(char **args, t_env **env_list)
{
	if (!args[1])
	{
		print_exported_vars(*env_list);
		return (0);
	}
	return (process_export_args(args, env_list));
}
