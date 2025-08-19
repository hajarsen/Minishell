/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jari <marvin@42.ma>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 23:00:11 by sel-jari          #+#    #+#             */
/*   Updated: 2025/08/13 23:00:24 by sel-jari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_nonfork_builtin(char **args, int *exit_status)
{
	if (is_builtin(args[0]) && (ft_strcmp(args[0], "cd") == 0
			|| ft_strcmp(args[0], "export") == 0
			|| ft_strcmp(args[0], "unset") == 0
			|| ft_strcmp(args[0], "exit") == 0))
	{
		execute_builtin(args, &glb_list()->env, exit_status);
		glb_list()->exit_status = *exit_status;
		return (1);
	}
	return (0);
}

void	execute_child_process(char **args, t_tokenizer *tokens)
{
	char		*path;
	char		**envp;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (execute_redirections(tokens))
	{
		clean_exit(1);
	}
	if (execute_builtin(args, &glb_list()->env, &glb_list()->exit_status) == 1)
	{
		clean_exit(glb_list()->exit_status);
	}
	path = get_cmd_path(args[0], glb_list()->env);
	handle_command_not_found(args[0], path);
	handle_directory_error(args[0], path);
	envp = envlist_to_array(glb_list()->env);
	if (!envp)
	{
		perror("envlist_to_array");
		clean_exit(1);
	}
	handle_execve_error_for_main(args, path, envp);
}

void	handle_parent_process_main(pid_t pid, int *exit_status)
{
	int	sig;

	if (pid > 0)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		waitpid(pid, exit_status, 0);
		setup_signals();
		if (WIFEXITED(*exit_status))
			glb_list()->exit_status = WEXITSTATUS(*exit_status);
		else if (WIFSIGNALED(*exit_status))
		{
			sig = WTERMSIG(*exit_status);
			if (sig == SIGINT)
				ft_putstr_fd("\n", STDOUT_FILENO);
			else if (sig == SIGQUIT)
				ft_putstr_fd("Quit: (core dumped)\n", STDOUT_FILENO);
			glb_list()->exit_status = 128 + sig;
		}
	}
	else
	{
		perror("fork");
		glb_list()->exit_status = 1;
	}
}
