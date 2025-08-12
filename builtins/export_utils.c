/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 05:25:12 by hsennane          #+#    #+#             */
/*   Updated: 2025/08/12 05:25:15 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_exported_vars(t_env *env)
{
	while (env)
	{
		if (env->value)
		{
			ft_putstr_fd("declare -x ", 1);
			ft_putstr_fd(env->name, 1);
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(env->value, 1);
			ft_putstr_fd("\"\n", 1);
		}
		else
		{
			ft_putstr_fd("declare -x ", 1);
			ft_putstr_fd(env->name, 1);
			ft_putstr_fd("\n", 1);
		}
		env = env->next;
	}
}
