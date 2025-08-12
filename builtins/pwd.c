/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 04:46:31 by hsennane          #+#    #+#             */
/*   Updated: 2025/08/12 04:46:34 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pwd(int *exit_status)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (path == NULL)
	{
		perror("pwd");
		*exit_status = 1;
		return ;
	}
	else
	{
		ft_putendl_fd(path, STDOUT_FILENO);
		free(path);
		*exit_status = 0;
	}
}
