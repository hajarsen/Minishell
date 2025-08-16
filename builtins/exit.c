/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 04:36:28 by hsennane          #+#    #+#             */
/*   Updated: 2025/08/12 04:36:30 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_numeric_arg(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '-' || arg[i] == '+')
		i++;
	if (!arg[i])
		return (0);
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		i++;
	}
	return (1);
}

static void	exit_num_error(char *arg)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	exit(2);
}

static void	exit_arg_error(void)
{
	ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
}

void	ft_exit(char **args)
{
	long long	exit_code;
	int			overflow;

	ft_putstr_fd("exit\n", STDOUT_FILENO);
	if (args[1])
	{
		if (!is_numeric_arg(args[1]))
			exit_num_error(args[1]);
		if (args[2])
		{
			exit_arg_error();
			glb_list()->exit_status = 1;
			return ;
		}
		exit_code = my_ft_atoll(args[1], &overflow);
		if (overflow)
			exit_num_error(args[1]);
		exit((unsigned char)exit_code);
	}
	exit(glb_list()->exit_status);
}
