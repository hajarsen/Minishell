/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 05:16:01 by hsennane          #+#    #+#             */
/*   Updated: 2025/08/12 05:16:02 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*find_in_paths(char **paths, char *cmd)
{
	char		*full_path;
	int			i;
	struct stat	st;

	i = 0;
	while (paths[i])
	{
		full_path = str_concat_three(paths[i], "/", cmd);
		if (full_path && stat(full_path, &st) == 0)
		{
			return (full_path);
		}
		free(full_path);
		i++;
	}
	return (NULL);
}

static char	*search_path_in_env(char *cmd, t_env *env_list)
{
	char		**paths;
	char		*path_env;
	char		*found;

	path_env = get_env_value("PATH", env_list);
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	found = find_in_paths(paths, cmd);
	if (found)
	{
		free_strs(paths);
		return (found);
	}
	free_strs(paths);
	return (NULL);
}

char	*get_cmd_path(char *cmd, t_env *env_list)
{
	char	*dup;

	if (!cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		dup = ft_strdup(cmd);
		return (dup);
	}
	return (search_path_in_env(cmd, env_list));
}

static int	child_process_exec(char **args, t_env **env)
{
	char	**envp;

	envp = envlist_to_array(*env);
	if (!envp)
	{
		ft_putstr_fd("minishell: failed to get environment\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	execve(args[0], args, envp);
	ft_putstr_fd("minishell: execve failed\n", STDERR_FILENO);
	exit(EXIT_FAILURE);
}

int	execute_command(char **args, t_env **env)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
	{
		ft_putstr_fd("minishell: fork failed\n", STDERR_FILENO);
		return (-1);
	}
	if (pid == 0)
		child_process_exec(args, env);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (0);
}
