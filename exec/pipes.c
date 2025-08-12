/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 05:16:37 by hsennane          #+#    #+#             */
/*   Updated: 2025/08/12 05:16:41 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	setup_child_pipes(int idx, int nseg, int (*pfds)[2])
{
	int	j;

	if (idx > 0)
	{
		if (dup2(pfds[idx - 1][READING_END], STDIN_FILENO) < 0)
		{
			ft_putendl_fd("minishell: dup2 failed", 2);
			exit(1);
		}
	}
	if (idx < nseg - 1)
	{
		if (dup2(pfds[idx][WRITING_END], STDOUT_FILENO) < 0)
		{
			ft_putendl_fd("minishell: dup2 failed", 2);
			exit(1);
		}
	}
	j = 0;
	while (j < nseg - 1)
	{
		close(pfds[j][READING_END]);
		close(pfds[j][WRITING_END]);
		j++;
	}
}

static void	apply_redirections_or_exit(t_tokenizer *seg_head)
{
	if (execute_redirections(seg_head))
		exit(1);
}

static void	perform_execve(char **args, char *path, t_env *env)
{
	char	**envp;
	int		saved_errno;

	envp = envlist_to_array(env);
	if (!envp)
	{
		ft_putendl_fd("minishell: envlist_to_array failed", 2);
		free(path);
		exit(1);
	}
	execve(path, args, envp);
	saved_errno = errno;
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(args[0], 2);
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

static void	handle_command_errors(char *cmd, char *path)
{
	struct stat	path_stat;

	if (!path)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putendl_fd(cmd, 2);
		exit(127);
	}
	if (stat(path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		free(path);
		exit(126);
	}
}

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
	handle_command_errors(args[0], path);
	perform_execve(args, path, glb->env);
}

static void	child_exec_segment(int idx, int nseg, int (*pfds)[2],
		t_tokenizer *seg_head, t_tokenizer *seg_tail, t_glb *glb,
		int *exit_status)
{
	char	**args;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (seg_tail)
		seg_tail->next = NULL;
	setup_child_pipes(idx, nseg, pfds);
	apply_redirections_or_exit(seg_head);
	args = tokens_to_args(seg_head);
	if (!args || !args[0])
		exit(0);
	exec_resolved_command(args, glb, exit_status);
}

static void	child_exec_single(t_tokenizer *seg_head, t_glb *glb,
		int *exit_status)
{
	char	**args;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	apply_redirections_or_exit(seg_head);
	args = tokens_to_args(seg_head);
	if (!args || !args[0])
		exit(0);
	exec_resolved_command(args, glb, exit_status);
}

static void	cleanup_pipes(int (*pfds)[2], int count)
{
	while (count-- > 0)
	{
		close(pfds[count][READING_END]);
		close(pfds[count][WRITING_END]);
	}
	free(pfds);
}

static int	create_single_pipe(int pipefd[2])
{
	if (pipe(pipefd) == -1)
	{
		ft_putstr_fd("minishell: pipe failed\n", 2);
		return (-1);
	}
	return (0);
}

static int	create_pipes(int nseg, int (**out_pfds)[2])
{
	int	i;

	int(*pfds)[2];
	*out_pfds = NULL;
	if (nseg <= 1)
		return (0);
	pfds = (int(*)[2])malloc(sizeof(int[2]) * (nseg - 1));
	if (!pfds)
	{
		ft_putstr_fd("minishell: malloc failed\n", 2);
		return (-1);
	}
	i = 0;
	while (i < nseg - 1)
	{
		if (create_single_pipe(pfds[i]) == -1)
		{
			cleanup_pipes(pfds, i);
			return (-1);
		}
		i++;
	}
	*out_pfds = pfds;
	return (0);
}

static int	alloc_pids(pid_t **pids, int nseg)
{
	*pids = (pid_t *)malloc(sizeof(pid_t) * nseg);
	if (!*pids)
	{
		ft_putendl_fd("minishell: malloc failed", 2);
		return (-1);
	}
	return (0);
}

static void	free_pipes(int (*pfds)[2], int n_pipes)
{
	if (!pfds)
		return ;
	close_all_pipes(pfds, n_pipes);
	free(pfds);
}

static void	wait_children(pid_t *pids, int nseg, int *last_status)
{
	int	i;
	int	status;

	*last_status = 0;
	i = 0;
	while (i < nseg)
	{
		if (pids[i] > 0)
		{
			waitpid(pids[i], &status, 0);
			if (i == nseg - 1)
				*last_status = status;
		}
		i++;
	}
}

static int	init_segments(t_tokenizer *tokens, t_tokenizer ***o_starts,
		t_tokenizer ***o_ends, int *o_nseg)
{
	int			nseg;
	t_tokenizer	**starts;
	t_tokenizer	**ends;

	nseg = count_segments(tokens);
	if (nseg <= 0)
		return (-1);
	starts = gc_alloc(sizeof(t_tokenizer *) * nseg);
	ends = gc_alloc(sizeof(t_tokenizer *) * nseg);
	if (!starts || !ends)
		return (-1);
	collect_segments(tokens, starts, ends, nseg);
	*o_nseg = nseg;
	*o_starts = starts;
	*o_ends = ends;
	return (0);
}

static void	spawn_children(int nseg, int (*pfds)[2], t_tokenizer **starts,
		t_tokenizer **ends, t_glb *glb, int *exit_status, pid_t *pids)
{
	int		i;
	pid_t	pid;

	i = 0;
	while (i < nseg)
	{
		pid = fork();
		if (pid < 0)
		{
			ft_putendl_fd("minishell: fork failed", 2);
			pids[i] = -1;
		}
		else if (pid == 0)
		{
			if (nseg > 1)
				child_exec_segment(i, nseg, pfds, starts[i], ends[i], glb,
					exit_status);
			else
				child_exec_single(starts[i], glb, exit_status);
		}
		pids[i] = pid;
		i++;
	}
}

static void	finalize_parent(int nseg, int (*pfds)[2], pid_t *pids,
		int *exit_status, t_glb *glb)
{
	int	last_status;

	ignore_interactive_signals();
	free_pipes(pfds, nseg - 1);
	wait_children(pids, nseg, &last_status);
	free(pids);
	extract_exit_status(last_status, exit_status);
	save_exit_status(glb, *exit_status);
}

void	execute_pipeline(t_tokenizer *tokens, t_glb *glb, int *exit_status)
{
	int			nseg;
	t_tokenizer	**starts;
	t_tokenizer	**ends;
	pid_t		*pids;

	int(*pfds)[2];
	if (init_segments(tokens, &starts, &ends, &nseg) < 0)
		return ;
	if (create_pipes(nseg, &pfds) < 0)
		return ;
	if (alloc_pids(&pids, nseg) < 0)
	{
		free_pipes(pfds, nseg - 1);
		return ;
	}
	spawn_children(nseg, pfds, starts, ends, glb, exit_status, pids);
	finalize_parent(nseg, pfds, pids, exit_status, glb);
}
