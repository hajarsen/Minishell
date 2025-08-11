#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

int main()
{
    char *input = readline("\033[1;32m➜\033[0m\033[1;36m Minishell@damn:$\033[0m ");
    free(input);

    // Cleanup readline allocations (optional but useful for Valgrind)
	
    return 0;
}

