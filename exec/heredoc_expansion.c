/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expansion.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 05:16:27 by hsennane          #+#    #+#             */
/*   Updated: 2025/08/12 05:16:28 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*handle_special_var(const char *p, size_t *consumed)
{
	if (p[1] == '?')
	{
		*consumed = 2;
		return (ft_itoa(glb_list()->exit_status));
	}
	if (ft_isdigit((unsigned char)p[1]))
	{
		*consumed = 2;
		return (ft_strdup(""));
	}
	if (!is_name_char((unsigned char)p[1]))
	{
		*consumed = 1;
		return (ft_strdup("$"));
	}
	return (NULL);
}

static char	*expand_name_var(const char *p, size_t *consumed, t_env *env)
{
	char	*name;
	char	*val;
	char	*ret;
	size_t	i;

	i = 1;
	while (is_name_char((unsigned char)p[i]))
		i++;
	name = ft_substr(p, 1, i - 1);
	val = get_env_value(name, env);
	free(name);
	if (val)
		ret = ft_strdup(val);
	else
		ret = ft_strdup("");
	*consumed = i;
	return (ret);
}

static char	*expand_var(const char *p, size_t *consumed, t_env *env)
{
	char	*ret;

	ret = handle_special_var(p, consumed);
	if (ret)
		return (ret);
	return (expand_name_var(p, consumed, env));
}

static char	*join_and_free(char *out, char *frag)
{
	char	*tmp;

	tmp = ft_strjoin(out, frag);
	free(out);
	free(frag);
	return (tmp);
}

char	*expand_heredoc_line(const char *line, t_env *env)
{
	size_t	i;
	size_t	used;
	char	*out;
	char	*frag;
	char	*buf;

	i = 0;
	out = ft_strdup("");
	while (line && line[i])
	{
		if (line[i] == '$')
		{
			frag = expand_var(line + i, &used, env);
			out = join_and_free(out, frag);
			i += used;
			continue ;
		}
		buf = ft_substr(line, i, 1);
		out = join_and_free(out, buf);
		i++;
	}
	return (out);
}
