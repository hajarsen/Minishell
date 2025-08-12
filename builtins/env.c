/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 04:36:17 by hsennane          #+#    #+#             */
/*   Updated: 2025/08/12 04:36:18 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_env_var(t_env *env)
{
	if (env->value)
	{
		ft_putstr_fd(env->name, STDOUT_FILENO);
		ft_putstr_fd("=", STDOUT_FILENO);
		ft_putendl_fd(env->value, STDOUT_FILENO);
	}
}

static void	print_env_error(char *var)
{
	ft_putstr_fd("env: ", STDERR_FILENO);
	ft_putstr_fd(var, STDERR_FILENO);
	ft_putendl_fd(": No such file or directory", STDERR_FILENO);
}

static int	print_var_value(t_env *env_list, char *name)
{
	t_env	*tmp;

	tmp = env_list;
	while (tmp)
	{
		if (ft_strcmp(tmp->name, name) == 0)
		{
			if (tmp->value)
			{
				ft_putendl_fd(tmp->value, STDOUT_FILENO);
				return (0);
			}
			break ;
		}
		tmp = tmp->next;
	}
	print_env_error(name);
	return (127);
}

int	ft_env(char **args, t_env *env_list)
{
	int		status;
	t_env	*tmp;

	status = 0;
	if (args[1])
		return (print_var_value(env_list, args[1]));
	tmp = env_list;
	while (tmp)
	{
		print_env_var(tmp);
		tmp = tmp->next;
	}
	return (status);
}
