#include "minishell.h"


int is_builtin(const char *cmd)
{
	if (!cmd)
		return (0);
	if (!ft_strcmp(cmd, "cd"))
		return (1);
	if (!ft_strcmp(cmd, "exit"))
		return (1);
	if (!ft_strcmp(cmd, "env"))
		return (1);
	if (!ft_strcmp(cmd, "export"))
		return (1);
	if (!ft_strcmp(cmd, "unset"))
		return (1);
	if (!ft_strcmp(cmd, "pwd"))
		return (1);
	if (!ft_strcmp(cmd, "echo"))
		return (1);
	return (0);
}
int execute_builtin(char **args, t_env **env_list, int *exit_status)
{
	if (!args || !args[0])
		return (0);
	if (!ft_strcmp(args[0], "cd"))
		*exit_status = ft_cd(args, env_list);
	else if (!ft_strcmp(args[0], "exit"))
		ft_exit(args);
	else if (!ft_strcmp(args[0], "env"))
		*exit_status = ft_env(args, *env_list);
	else if (!ft_strcmp(args[0], "export"))
		*exit_status = ft_export(args, env_list);
	else if (!ft_strcmp(args[0], "unset"))
		builtin_unset(args, env_list);
	else if (!ft_strcmp(args[0], "pwd"))
		pwd(exit_status);
	else if (!ft_strcmp(args[0], "echo"))
		echo(args, exit_status);
	else
		return (0);
	return (1);
}
