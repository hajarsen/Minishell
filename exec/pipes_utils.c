#include "minishell.h"

void save_exit_status(t_glb *glb, int status_code)
{
    int fd;
    ssize_t bytes_written;

    fd = open(SHELL_CHILD_PID_FILE, O_WRONLY | O_CREAT | O_APPEND, 0777);
    if (fd < 0)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }
    bytes_written = write(fd, &status_code, sizeof(status_code));
    if (bytes_written < 0)
    {
        perror("write");
        exit(EXIT_FAILURE);
    }
    close(fd);
    glb->exit_status = status_code;
}

int has_pipe(t_tokenizer *tokens)
{
    while (tokens)
    {
        if (tokens->op == PIPE)
            return 1;
        tokens = tokens->next;
    }
    return 0;
}
