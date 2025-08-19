/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_update.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 05:10:10 by hsennane          #+#    #+#             */
/*   Updated: 2025/08/12 05:10:11 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	update_env_var(t_env *env_list, t_env *new_node)
{
	t_env	*tmp;
	char	*new_value;

	tmp = env_list;
	while (tmp)
	{
		if (ft_strcmp(tmp->name, new_node->name) == 0)
		{
			if (!new_node->value)
				return (1);
			new_value = ft_strdup_(new_node->value);
			if (!new_value)
				return (1);
			free(tmp->value);
			tmp->value = new_value;
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

static int	digit_nbr(int n)
{
	int	count;

	count = 1;
	while (n / 10 != 0)
	{
		n = n / 10;
		count++;
	}
	return (count);
}

static void	to_char(char *ptr, long l, int number_of_chars)
{
	ptr[number_of_chars] = 0;
	if (!l)
		ptr[0] = '0';
	while (l)
	{
		ptr[number_of_chars - 1] = l % 10 + 48;
		l = l / 10;
		number_of_chars--;
	}
}

char	*ft_itoa_(int n)
{
	char	*ptr;
	int		number_of_chars;
	long	l;

	l = n;
	number_of_chars = digit_nbr(l);
	if (l < 0)
	{
		number_of_chars++;
		ptr = malloc(number_of_chars + 1);
		if (!ptr)
			return (0);
		ptr[0] = '-';
		l *= -1;
	}
	else
		ptr = malloc(number_of_chars + 1);
	if (!ptr)
		return (0);
	to_char(ptr, l, number_of_chars);
	return (ptr);
}

void	update_shell_lvl(t_env **env_list)
{
	char	*shell_lvl;
	int		lvl;
	char	*n_lv_str;

	shell_lvl = get_env_value("SHLVL", *env_list);
	lvl = 1;
	if (shell_lvl)
		lvl = ft_atoi(shell_lvl) + 1;
	n_lv_str = ft_itoa_(lvl);
	if (!n_lv_str)
		return ;
	insert_env_node(env_list, create_env_node(ft_strdup_("SHLVL"), n_lv_str));
}
