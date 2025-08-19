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
	if (!path || arg[0] == 0)
	{
		if (ft_strchr(arg, '/') || !get_env_value("PATH", glb_list()->env))
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(arg, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
		}
		else
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(arg, 2);
			ft_putstr_fd(": command not found", 2);
			ft_putchar_fd('\n', 2);
		}
		glb_list()->exit_status = 127;
		gc_free_all();
		free_env(glb_list()->env);
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
		gc_free_all();
		free_env(glb_list()->env);
		exit(126);
	}
}

void	handle_execve_error_for_main(char **args, char *path, char **envp)
{
	if (execve(path, args, envp) == -1)
	{
		ft_putstr_fd("minishell:", 2);
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(": Permission denied\n", 2);
		gc_free_all();
		free_env(glb_list()->env);
		exit(126);
	}
}
