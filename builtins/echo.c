#include "minishell.h"

static int check_n_flag(char *str)
{
	int i;

	i = 1;
	if (!str || (str[0] != '-') || (str[1] == '\0'))
		return 0;
	while (str[i])
	{
		if (str[i] != 'n')
			return 0;
		i++;
	}
	return 1;
}

void echo(char **args, int *exit_status)
{
    int i = 1;
    int new_line = 1;

    if (args[i] && ft_strcmp(args[i], "$?") == 0)
    {
        printf("%d\n", glb_list()->exit_status);
        *exit_status = 0;
        return;
    }

    while (args[i] && check_n_flag(args[i]))
    {
        new_line = 0;
        i++;
    }
    while (args[i])
    {
        ft_putstr_fd(args[i], STDOUT_FILENO);
        if (args[i + 1])
            ft_putstr_fd(" ", STDOUT_FILENO);
        i++;
    }
    if (new_line)
        ft_putstr_fd("\n", STDOUT_FILENO);
    *exit_status = 0;
}