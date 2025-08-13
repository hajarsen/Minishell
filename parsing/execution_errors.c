/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_errors.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jari <marvin@42.ma>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 22:33:58 by sel-jari          #+#    #+#             */
/*   Updated: 2025/08/13 22:34:01 by sel-jari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_command_not_found(char *arg, char *path)
{
	if (!path)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putchar_fd('\n', 2);
		glb_list()->exit_status = 127;
		exit(127);
	}
}

void	handle_directory_error(char *cmd_name, char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd_name, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		free(path);
		exit(126);
	}
}

void	handle_execve_error_for_main(char *cmd_name, char *path, char **envp)
{
	int	saved_errno;

	saved_errno = errno;
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd_name, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(strerror(saved_errno), 2);
	free(path);
	free_strs(envp);
	if (saved_errno == ENOENT)
		exit(127);
	else if (saved_errno == EACCES || saved_errno == ENOTDIR
		|| saved_errno == EISDIR || saved_errno == ENOEXEC)
		exit(126);
	else
		exit(126);
}
