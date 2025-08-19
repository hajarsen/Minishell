/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 04:45:49 by hsennane          #+#    #+#             */
/*   Updated: 2025/08/13 04:45:52 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_execve_error(char **args, char *path)
{
	(void)path;
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(args[0], 2);
	ft_putstr_fd(": Permission denied\n", 2);
	free_strs(envlist_to_array(NULL));
	gc_free_all();
	exit(126);
}

void	perform_execve(char **args, char *path, t_env *env)
{
	char	**envp;

	envp = envlist_to_array(env);
	if (!envp)
	{
		ft_putendl_fd("minishell: envlist_to_array failed", 2);
		gc_free_all();
		free_env(glb_list()->env);
		exit(1);
	}
	execve(path, args, envp);
	handle_execve_error(args, path);
}

int	has_pipe(t_tokenizer *tokens, char *input, int *exit_status)
{
	t_tokenizer	*tmp;

	tmp = tokens;
	while (tokens)
	{
		if (tokens->op == PIPE)
		{
			execute_pipeline(tmp, glb_list(), exit_status);
			close_redirection_fds(tmp);
			gc_free_all();
			return (1);
		}
		tokens = tokens->next;
	}
	(void)input;
	return (0);
}

void	ignore_interactive_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}
