/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jari <marvin@42.ma>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 01:34:07 by sel-jari          #+#    #+#             */
/*   Updated: 2025/08/13 01:34:09 by sel-jari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*check_env(char *str, int *check)
{
	t_env	*env;
	char	*exit_str;
	char	*temp_itoa;

	if (ft_strncmp(str, "?", 2) == 0)
	{
		temp_itoa = ft_itoa(glb_list()->exit_status);
		exit_str = gc_alloc(ft_strlen(temp_itoa) + 1);
		ft_strlcpy(exit_str, temp_itoa, ft_strlen(temp_itoa) + 1);
		free_str_and_itoa(temp_itoa, str);
		*check = 0;
		return (exit_str);
	}
	env = glb_list()->env;
	while (env != 0)
	{
		if (ft_strncmp(str, env->name, ft_strlen(env->name) + 1) == 0)
		{
			free(str);
			return (env->value);
		}
		env = env->next;
	}
	free(str);
	return (NULL);
}

char	*new_alloc(char *str, char *env_value, int *start, int len)
{
	int		new_len;
	char	*new_str;
	int		new_start;

	new_len = ft_strlen(str) - len + ft_strlen(env_value);
	new_str = gc_alloc(new_len + 1);
	ft_strlcpy(new_str, str, *start + 1);
	ft_strlcat(new_str, env_value, new_len + 1);
	new_start = ft_strlen(new_str);
	ft_strlcat(new_str, str + *start + len, new_len + 1);
	*start = new_start - 1;
	return (new_str);
}

char	*re_alloc(char *str, int *start, int len, char *env_value)
{
	char	*new_str;

	if (env_value == NULL)
	{
		ft_memmove(str + *start, str + *start + len,
			ft_strlen(str + *start + len) + 1);
		*start -= 1;
		return (str);
	}
	if (env_value[0] == '\0')
	{
		ft_memmove(str + *start, str + *start + len,
			ft_strlen(str + *start + len) + 1);
		*start -= 1;
		return (str);
	}
	new_str = new_alloc(str, env_value, start, len);
	free(str);
	return (new_str);
}

t_here_doc	*here_doc(t_tokenizer *token)
{
	char		*delemeter;
	t_here_doc	*hd;
	t_here_doc	*head;

	hd = malloc(sizeof(t_here_doc));
	head = hd;
	hd->str = token->next->str;
	delemeter = hd->str;
	hd->next = gc_alloc(sizeof(t_here_doc));
	hd = hd->next;
	hd->str = readline("> ");
	while (ft_strncmp(delemeter, hd->str, ft_strlen(hd->str)) != 0)
	{
		hd->next = gc_alloc(sizeof(t_here_doc));
		hd = hd->next;
		hd->str = readline("> ");
	}
	hd->next = NULL;
	return (head);
}

void	tokenize_the_envar(t_tokenizer **token)
{
	t_tokenizer	**token_temp;
	t_tokenizer	*token_next;
	t_tokenizer	*token_dele;
	char		*str;

	token_dele = *token;
	str = (*token)->str;
	token_next = (*token)->next;
	*token = tokenizer_for_expanding((*token)->str);
	free(str);
	free(token_dele);
	token_temp = token;
	while ((*token_temp) != NULL)
	{
		token_temp = &(*token_temp)->next;
	}
	*token_temp = token_next;
}
