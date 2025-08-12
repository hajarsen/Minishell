/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 04:36:02 by hsennane          #+#    #+#             */
/*   Updated: 2025/08/12 04:36:03 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	update_oldpwd(t_env **env_list, char *old_pwd)
{
	t_env	*oldpwd_node;
	char	*value;

	if (old_pwd)
		value = ft_strdup(old_pwd);
	else
		value = ft_strdup("");
	oldpwd_node = create_env_node(ft_strdup("OLDPWD"), value);
	if (!update_env_var(*env_list, oldpwd_node))
	{
		insert_env_node(env_list, oldpwd_node);
		return (0);
	}
	free(oldpwd_node->name);
	free(oldpwd_node->value);
	free(oldpwd_node);
	return (0);
}

static int	update_pwd(t_env **env_list)
{
	char	cwd[PATH_MAX];
	t_env	*pwd_node;

	if (!getcwd(cwd, sizeof(cwd)))
	{
		ft_putstr_fd("minishell: cd: error retrieving current directory\n",
			STDERR_FILENO);
		return (1);
	}
	pwd_node = create_env_node(ft_strdup("PWD"), ft_strdup(cwd));
	if (!update_env_var(*env_list, pwd_node))
	{
		insert_env_node(env_list, pwd_node);
		return (0);
	}
	free(pwd_node->name);
	free(pwd_node->value);
	free(pwd_node);
	return (0);
}

static int	change_directory_and_update(t_env **env_list, const char *path)
{
	char	*old_pwd;

	old_pwd = get_env_value("PWD", *env_list);
	if (chdir(path) != 0)
	{
		print_cd_error(path);
		return (1);
	}
	if (update_oldpwd(env_list, old_pwd))
		return (1);
	if (update_pwd(env_list))
		return (1);
	return (0);
}

static int	cd_with_arg(t_env **env_list, char *arg)
{
	char	*oldpwd;

	if (ft_strcmp(arg, "-") == 0)
	{
		oldpwd = get_env_value("OLDPWD", *env_list);
		if (!oldpwd)
		{
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", STDERR_FILENO);
			return (1);
		}
		ft_putstr_fd(oldpwd, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		if (change_directory_and_update(env_list, oldpwd))
			return (1);
		return (0);
	}
	return (change_directory_and_update(env_list, arg));
}

int	ft_cd(char **args, t_env **env_list)
{
	char	*target;

	if (!args[1])
	{
		target = get_env_value_or_default(*env_list, "HOME", "/");
		if (!target)
			return (1);
		if (change_directory_and_update(env_list, target))
		{
			free(target);
			return (1);
		}
		free(target);
		return (0);
	}
	if (args[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	return (cd_with_arg(env_list, args[1]));
}
