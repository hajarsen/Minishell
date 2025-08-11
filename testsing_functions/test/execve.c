#include "../minishell.h"
#include <string.h>
int main()
{
    char    *input;
	char	*arg[] = {"ls", NULL};
	int		id;

        input = readline("\033[1;32m➜\033[0m\033[1;36m Minishell@damn:$\033[0m ");
			free(input);
	rl_clear_history();
	rl_cleanup_after_signal();
}
221