/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 06:03:00 by hsennane          #+#    #+#             */
/*   Updated: 2025/08/12 06:03:04 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_heredoc_signals(void)
{
	struct sigaction	act;

	sigemptyset(&act.sa_mask);
	act.sa_handler = SIG_DFL;
	act.sa_flags = 0;
	sigaction(SIGINT, &act, NULL);
	act.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &act, NULL);
}

int	write_line_to_pipe(int write_fd, char *line)
{
	if (write(write_fd, line, ft_strlen(line)) == -1 || write(write_fd, "\n",
			1) == -1)
	{
		ft_putstr_fd("minishell: write error\n", 2);
		free(line);
		return (-1);
	}
	free(line);
	return (0);
}

int	process_heredoc_line(t_tokenizer *token, t_env *env, int write_fd,
		char *line)
{
	char	*expanded;
	int		ret;

	if (ft_strcmp(line, token->str) == 0)
	{
		free(line);
		return (1);
	}
	expanded = line;
	if (!heredoc_delimiter_is_quoted(token))
	{
		expanded = expand_heredoc_line(line, env);
		free(line);
	}
	ret = write_line_to_pipe(write_fd, expanded);
	if (ret == -1)
	{
		return (-1);
	}
	return (0);
}
