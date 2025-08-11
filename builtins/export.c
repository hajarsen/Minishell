#include "minishell.h"

static int is_valid_identifier(const char *s)
{
	int i;

	i = 0;
	if (!s || !*s)
		return (0);
	/* First character must be alpha or underscore */
	if (!(ft_isalpha(s[i]) || s[i] == '_'))
		return (0);
	/* Handle special case where the identifier is just a dash (A-) */
	if (s[i + 1] == '-' && (s[i + 2] == '=' || s[i + 2] == '\0'))
		return (0);
	while (s[i] && s[i] != '=')
	{
		/* Only alphanumeric and underscore allowed in variable names */
		if (!(ft_isalnum(s[i]) || s[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

static void print_exported_vars(t_env *env)
{
	while (env)
	{
		if (env->value)
		{
			ft_putstr_fd("declare -x ", 1);
			ft_putstr_fd(env->name, 1);
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(env->value, 1);
			ft_putstr_fd("\"\n", 1);
		}
		else
		{
			ft_putstr_fd("declare -x ", 1);
			ft_putstr_fd(env->name, 1);
			ft_putstr_fd("\n", 1);
		}
		env = env->next;
	}
}

static void update_or_add_export(char *arg, t_env **env_list)
{
	char *eq;
	char *name;
	char *value;
	t_env *new_node;

	eq = ft_strchr(arg, '=');
	name = NULL;
	value = NULL;
	if (eq)
	{
		name = ft_substr(arg, 0, eq - arg);
		value = ft_strdup(eq + 1);
	}
	else
		name = ft_strdup(arg);
	if (name)
	{
		new_node = create_env_node(name, value);
		// hna kant khassak wa7d free hiya li kant dayra lmochkil kammel
		if (update_env_var(*env_list, new_node))
		{
			free(new_node->name);
			free(new_node->value);
			free(new_node);
		}
		else
		{
			insert_env_node(env_list, new_node);
		}
	}
}

int ft_export(char **args, t_env **env_list)
{
	int i;
	int ret_val;

	i = 1;
	ret_val = 0;
	if (!args[1])
	{
		print_exported_vars(*env_list);
		return (0);
	}
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			ret_val = 1; /* Set exit code to 1 for invalid identifiers */
		}
		else
			update_or_add_export(args[i], env_list);
		i++;
	}
	return (ret_val);
}

