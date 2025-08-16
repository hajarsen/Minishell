/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 05:24:50 by hsennane          #+#    #+#             */
/*   Updated: 2025/08/12 05:24:51 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_cd_error(const char *path)
{
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	ft_putstr_fd((char *)path, STDERR_FILENO);
	ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
}

char	*get_env_value_or_default(t_env *env_list, const char *name,
		const char *default_val)
{
	char	*val;
	char	*copy;

	val = get_env_value((char *)name, env_list);
	if (!val)
		copy = ft_strdup(default_val);
	else
		copy = ft_strdup(val);
	return (copy);
}
