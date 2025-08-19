/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_exec_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 02:51:32 by sel-jari          #+#    #+#             */
/*   Updated: 2025/08/18 02:51:35 by sel-jari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	print_minishell_err_cmd(const char *cmd, char *buf, int pos,
		int len)
{
	if (cmd && *cmd)
	{
		ft_strlcpy(buf + pos, ": ", len - pos);
		pos += ft_strlen(": ");
		ft_strlcpy(buf + pos, cmd, len - pos);
		pos += ft_strlen(cmd);
	}
	return (pos);
}

void	print_minishell_err(const char *cmd, const char *msg)
{
	int		len;
	int		pos;
	char	*buf;

	len = ft_strlen("minishell");
	if (cmd && *cmd)
		len += 2 + ft_strlen(cmd);
	len += 3 + ft_strlen(msg);
	buf = gc_alloc(len + 1);
	pos = 0;
	ft_strlcpy(buf + pos, "minishell", len - pos);
	pos += ft_strlen("minishell");
	pos = print_minishell_err_cmd(cmd, buf, pos, len);
	ft_strlcpy(buf + pos, ": ", len - pos);
	pos += ft_strlen(": ");
	ft_strlcpy(buf + pos, msg, len - pos);
	pos += ft_strlen(msg);
	buf[pos] = '\n';
	buf[pos + 1] = '\0';
	write(2, buf, pos + 1);
}
