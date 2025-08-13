#include "minishell.h"

t_env	*save_env(char **env)
{
	t_env	*new_node;
	t_env	*head;
	t_env	*tail;
	int		i;
	int		j;

	i = 0;
	head = NULL;
	tail = NULL;
	if (!env)
		return (NULL);
	while (env[i] != 0)
	{
		new_node = gc_alloc(sizeof(t_env));
		if (!new_node)
			return (NULL);
		j = 0;
		while (env[i][j] && env[i][j] != '=')
			j++;
		if (env[i][j] == '=')
		{
			new_node->name = ft_substr(env[i], 0, j);
			new_node->value = ft_substr(env[i], j + 1, ft_strlen(env[i]));
		}
		else
		{
			new_node->name = ft_strdup(env[i]);
			new_node->value = ft_strdup("");
		}
		new_node->next = NULL;
		if (head == NULL)
		{
			head = new_node;
			tail = new_node;
		}
		else
		{
			tail->next = new_node;
			tail = new_node;
		}
		i++;
	}
	return (head);
}

int	main(int ac, char **av, char **env)
{
	t_tokenizer	*tokens;
	char		*input;
	char		**args;
	int			exit_status;
	char		**envp;
	char		*path;
	pid_t		pid;
	struct stat	path_stat;
	int			saved_errno;

	(void)ac;
	(void)av;
	setup_signals();
	glb_list()->env = save_env(env);
	glb_list()->exit_status = 0;
	while (1)
	{
		input = readline("\033[1;32m➜\033[0m\033[1;36m Minishell $> \033[0m");
		if (!input)
		{
			printf("exit\n");
			free_env(glb_list()->env);
			rl_clear_history();
			exit(glb_list()->exit_status);
		}
		if (ft_strlen(input) == 0)
		{
			free(input);
			continue ;
		}
		if (input_error(input) == 1)
		{
			free(input);
			continue ;
		}
		add_history(input);
		tokens = tokenizer(input);
		if (check_parsing_errors(tokens))
		{
			free_tokens(input, tokens);
			continue ;
		}
		expanding(&tokens);
		set_signal_handler(tokens);
		init_redirect_fds(tokens);
		redirection_infos(tokens);
		if (has_pipe(tokens))
		{
			execute_pipeline(tokens, glb_list(), &exit_status);
			close_redirection_fds(tokens);
			free_tokens(input, tokens);
			continue ;
		}
		args = tokens_to_args(tokens);
		if (args && args[0] && args[0][0] != 0)
		{
			exit_status = 0;
			if (is_builtin(args[0]) && (ft_strcmp(args[0], "cd") == 0
					|| ft_strcmp(args[0], "export") == 0
					|| ft_strcmp(args[0], "unset") == 0
					|| ft_strcmp(args[0], "exit") == 0))
			{
				execute_builtin(args, &glb_list()->env, &exit_status);
				glb_list()->exit_status = exit_status;
			}
			else
			{
				pid = fork();
				if (pid == 0)
				{
					if (execute_redirections(tokens))
						exit(1);
					if (execute_builtin(args, &glb_list()->env, &exit_status) == 1)
						exit(exit_status);
					path = get_cmd_path(args[0], glb_list()->env);
					if (!path)
					{
						ft_putstr_fd("minishell: command not found: ", 2);
						ft_putstr_fd(args[0], 2);
						ft_putchar_fd('\n', 2);
						glb_list()->exit_status = 127;
						exit(127);
					}
					if (stat(path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
					{
						ft_putstr_fd("minishell: ", 2);
						ft_putstr_fd(args[0], 2);
						ft_putstr_fd(": Is a directory\n", 2);
						free(path);
						exit(126);
					}
					envp = envlist_to_array(glb_list()->env);
					if (!envp)
					{
						perror("envlist_to_array");
						free(path);
						exit(1);
					}
					execve(path, args, envp);
					{
						saved_errno = errno;
						ft_putstr_fd("minishell: ", 2);
						ft_putstr_fd(args[0], 2);
						ft_putstr_fd(": ", 2);
						ft_putendl_fd(strerror(saved_errno), 2);
						free(path);
						free_strs(envp);
						if (saved_errno == ENOENT)
							exit(127);
						else if (saved_errno == EACCES || saved_errno == ENOTDIR || saved_errno == EISDIR || saved_errno == ENOEXEC)
							exit(126);
						else
							exit(126);
					}
				}
				else if (pid > 0)
				{
					waitpid(pid, &exit_status, 0);
					if (WIFEXITED(exit_status))
						glb_list()->exit_status = WEXITSTATUS(exit_status);
					else if (WIFSIGNALED(exit_status))
						glb_list()->exit_status = 128 + WTERMSIG(exit_status);
				}
				else
				{
					perror("fork");
					glb_list()->exit_status = 1;
				}
			}
		}
		close_redirection_fds(tokens);
		free_args(args);
		free_tokens(input, tokens);
	}
	gc_free_all();
	return (glb_list()->exit_status);
}
