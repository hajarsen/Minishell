/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 05:16:47 by hsennane          #+#    #+#             */
/*   Updated: 2025/08/12 05:16:48 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	save_exit_status(t_glb *glb, int status_code)
{
	glb->exit_status = status_code;
}

int	has_pipe(t_tokenizer *tokens)
{
	while (tokens)
	{
		if (tokens->op == PIPE)
			return (1);
		tokens = tokens->next;
	}
	return (0);
}
