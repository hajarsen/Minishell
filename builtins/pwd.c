#include "minishell.h"

void pwd(int *exit_status) {
  char *path;

  path = getcwd(NULL, 0);
  if (path == NULL) {
    perror("pwd");
    *exit_status = 1;
    return;
  } else {
    ft_putendl_fd(path, STDOUT_FILENO);
    free(path);
    *exit_status = 0;
  }
}
