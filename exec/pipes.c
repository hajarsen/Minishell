/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 04:02:45 by hsennane          #+#    #+#             */
/*   Updated: 2025/08/13 04:02:50 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_resolved_command(char **args, t_glb *glb, int *exit_status)
{
	int		is_blt;
	char	*path;

	if (is_builtin(args[0]))
	{
		is_blt = execute_builtin(args, &glb->env, exit_status);
		if (is_blt)
			exit(*exit_status);
	}
	path = get_cmd_path(args[0], glb->env);
	if (!path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
	perform_execve(args, path, glb->env);
}

static void	child_executor(t_pipe_data *pipe_data, int i)
{
	char		**args;
	t_tokenizer	*seg_head;
	t_tokenizer	*seg_tail;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	seg_head = pipe_data->starts[i];
	seg_tail = pipe_data->ends[i];
	if (seg_tail)
		seg_tail->next = NULL;
	if (execute_redirections(seg_head))
		exit(1);
	args = tokens_to_args(seg_head);
	if (!args || !args[0])
		exit(0);
	exec_resolved_command(args, pipe_data->glb, pipe_data->exit_status);
}

static void	spawn_child(t_pipe_data *pipe_data, int i)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		ft_putendl_fd("minishell: fork failed", 2);
		pipe_data->pids[i] = -1;
	}
	else if (pid == 0)
	{
		if (pipe_data->nseg > 1)
			setup_child_pipes(i, pipe_data->nseg, pipe_data->pfds);
		child_executor(pipe_data, i);
	}
	pipe_data->pids[i] = pid;
}

static void	spawn_all_children(t_pipe_data *pipe_data)
{
	int	i;

	i = 0;
	while (i < pipe_data->nseg)
	{
		spawn_child(pipe_data, i);
		i++;
	}
}

void	execute_pipeline(t_tokenizer *tokens, t_glb *glb, int *exit_status)
{
	t_pipe_data	pipe_data;
	int			last_status;

	pipe_data.glb = glb;
	pipe_data.exit_status = exit_status;
	pipe_data.nseg = count_segments(tokens);
	if (pipe_data.nseg <= 0 || !init_pipeline_data(&pipe_data, tokens))
		return ;
	spawn_all_children(&pipe_data);
	ignore_interactive_signals();
	cleanup_pipeline(&pipe_data, &last_status);
	extract_exit_status(last_status, pipe_data.exit_status);
	save_exit_status(pipe_data.glb, *pipe_data.exit_status);
}
