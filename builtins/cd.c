#include "minishell.h"

static char *get_env_value_or_default(t_env *env_list, const char *name, const char *default_val)
{
    char *val;
    char *copy;

    val = get_env_value((char *)name, env_list);
    if (!val)
        copy = ft_strdup(default_val);
    else
        copy = ft_strdup(val);
    return copy;
}

static void print_cd_error(const char *path)
{
    ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
    ft_putstr_fd((char *)path, STDERR_FILENO);
    ft_putstr_fd(": ", STDERR_FILENO);
    ft_putstr_fd(strerror(errno), STDERR_FILENO);
    ft_putstr_fd("\n", STDERR_FILENO);
}

static int change_directory_and_update(t_env **env_list, const char *path)
{
    char *old_pwd;
    char cwd[PATH_MAX];
    t_env *oldpwd_node;
    t_env *pwd_node;

    old_pwd = get_env_value("PWD", *env_list);
    if (chdir(path) != 0)
    {
        print_cd_error(path);
        return 1;
    }
    if (!getcwd(cwd, sizeof(cwd)))
    {
        perror("minishell: getcwd");
        return 1;
    }
    
    oldpwd_node = create_env_node(ft_strdup("OLDPWD"), ft_strdup(old_pwd ? old_pwd : ""));
    if (!update_env_var(*env_list, oldpwd_node))
    {
        insert_env_node(env_list, oldpwd_node);
    }
    else
    {
        free(oldpwd_node->name);
        free(oldpwd_node->value);
        free(oldpwd_node);
    }
    
    pwd_node = create_env_node(ft_strdup("PWD"), ft_strdup(cwd));
    if (!update_env_var(*env_list, pwd_node))
    {
        insert_env_node(env_list, pwd_node);
    }
    else
    {
        free(pwd_node->name);
        free(pwd_node->value);
        free(pwd_node);
    }
    
    return 0;
}

static int cd_with_arg(t_env **env_list, char *arg)
{
    if (ft_strcmp(arg, "-") == 0)
    {
        char *oldpwd = get_env_value("OLDPWD", *env_list);
        if (!oldpwd)
        {
            ft_putstr_fd("minishell: cd: OLDPWD not set\n", STDERR_FILENO);
            return 1;
        }
        ft_putstr_fd(oldpwd, STDOUT_FILENO);
        ft_putstr_fd("\n", STDOUT_FILENO);
        if (change_directory_and_update(env_list, oldpwd))
        {
            return 1;
        }
        return 0;
    }
    return change_directory_and_update(env_list, arg);
}

int ft_cd(char **args, t_env **env_list)
{
    char *target;

    if (!args[1])
    {
        target = get_env_value_or_default(*env_list, "HOME", "/");
        if (!target)
            return 1;
        if (change_directory_and_update(env_list, target))
        {
            free(target);
            return 1;
        }
        free(target);
        return 0;
    }
    if (args[2])
    {
        ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
        return 2; /* Change exit code to 2 to match bash behavior */
    }
    return cd_with_arg(env_list, args[1]);
}

