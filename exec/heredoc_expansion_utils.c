/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expansion_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 06:29:30 by hsennane          #+#    #+#             */
/*   Updated: 2025/08/12 06:29:32 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_name_char(int c)
{
	return (ft_isalnum(c) || c == '_');
}

int	heredoc_delimiter_is_quoted(t_tokenizer *delim_tok)
{
	if (!delim_tok)
		return (0);
	if (delim_tok->redirect.qt == THERES_QUOTE)
		return (1);
	return (0);
}
