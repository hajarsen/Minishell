/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jari <marvin@42.ma>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:32:26 by sel-jari          #+#    #+#             */
/*   Updated: 2025/08/13 16:32:27 by sel-jari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execution(char **args, t_tokenizer *tokens)
{
	pid_t	pid;
	int		exit_status;

	exit_status = 0;
	if (!exec_nonfork_builtin(args, &exit_status))
	{
		pid = fork();
		if (pid == 0)
			execute_child_process(args, tokens);
		else
			handle_parent_process_main(pid, &exit_status);
	}
}

static void	parsing(char *input)
{
	t_tokenizer	*tokens;
	char		**args;
	int			exit_status;

	if (check_input_errors(input))
		return ;
	tokens = tokenizer(input);
	if (check_parsing_errors(tokens, input))
		return ;
	dont_expand_herdoc(tokens);
	expanding(&tokens);
	set_signal_handler(tokens);
	init_redirect_fds(tokens);
	redirection_infos(tokens);
	if (has_pipe(tokens, input, &exit_status))
		return ;
	args = tokens_to_args(tokens);
	if (args && args[0] && tokens->op != ITS_NULL_EXPAND)
		execution(args, tokens);
	close_redirection_fds(tokens);
	free_args(args);
	free_tokens(input, tokens);
}

int	main(int ac, char **av, char **env)
{
	char	*input;

	(void)ac;
	(void)av;
	setup_signals();
	glb_list()->env = save_env(env);
	glb_list()->exit_status = 0;
	while (1)
	{
		input = readline("\033[1;32m➜\033[0m\033[1;36m Minishell $> \033[0m");
		glb_list()->input = input;
		parsing(input);
	}
	gc_free_all();
	return (glb_list()->exit_status);
}
