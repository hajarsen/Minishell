#include "minishell.h"

static int is_numeric_arg(char *arg)
{
    int i;

    i = 0;
    if (arg[i] == '-' || arg[i] == '+')
        i++;
    if (!arg[i])
        return 0;
    while (arg[i])
    {
        if (!ft_isdigit(arg[i]))
            return 0;
        i++;
    }
    return 1;
}

static void exit_num_error(char *arg)
{
    ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
    ft_putstr_fd(arg, STDERR_FILENO);
    ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
    exit(2);
}

static void exit_arg_error(void)
{
    ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
}

void ft_exit(char **args)
{
    long long exit_code;
    int overflow;

    ft_putstr_fd("exit\n", STDOUT_FILENO);
    if (args[1])
    {
        if (!is_numeric_arg(args[1]))
            exit_num_error(args[1]); /* This now exits with exit code 2 */
        if (args[2])
        {
            exit_arg_error();
            exit(1); 
        }
        exit_code = my_ft_atoll(args[1], &overflow);
        if (overflow)
            exit_num_error(args[1]);
        exit((unsigned char)exit_code);
    }
    exit(0);
}
