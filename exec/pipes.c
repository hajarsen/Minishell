#include "minishell.h"

static void	signal_handler(void)
{
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}

static void extract_exit_status(int status, int *exit_status)
{
    if (WIFEXITED(status))
        *exit_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        *exit_status = 128 + WTERMSIG(status);
    else
        *exit_status = 1;
}

static void execute_left_side(t_tokenizer *left_tokens, t_glb *glb, int fd[2], int *exit_status)
{
    char **args = tokens_to_args(left_tokens);

    close(fd[READING_END]);
    dup2(fd[WRITING_END], STDOUT_FILENO);
    close(fd[WRITING_END]);

    if (args && args[0])
    {
        if (is_builtin(args[0]))
            exit(execute_builtin(args, &glb->env, exit_status));
        else
        {
            char *path = get_cmd_path(args[0], glb->env);
            if (!path)
            {
                ft_putstr_fd("minishell: command not found: ", 2);
                ft_putendl_fd(args[0], 2);
                exit(127);
            }
            execve(path, args, envlist_to_array(glb->env));
            perror("execve");
            exit(126);
        }
    }
    exit(0);
}

static void execute_right_side(t_tokenizer *right_tokens, t_glb *glb, int fd[2], int *exit_status)
{
    char **args = tokens_to_args(right_tokens);

    close(fd[WRITING_END]);
    dup2(fd[READING_END], STDIN_FILENO);
    close(fd[READING_END]);

    if (args && args[0])
    {
        if (is_builtin(args[0]))
            exit(execute_builtin(args, &glb->env, exit_status));
        else
        {
            char *path = get_cmd_path(args[0], glb->env);
            if (!path)
            {
                ft_putstr_fd("minishell: command not found: ", 2);
                ft_putendl_fd(args[0], 2);
                exit(127);
            }
            execve(path, args, envlist_to_array(glb->env));
            perror("execve");
            exit(126);
        }
    }
    exit(0);
}

void execute_pipeline(t_tokenizer *tokens, t_glb *glb, int *exit_status)
{
    int fd[2];
    pid_t pid1, pid2;
    int status;
    t_tokenizer *left_tokens = tokens;
    t_tokenizer *right_tokens = NULL;
    t_tokenizer *tmp = tokens;

    while (tmp && tmp->next && tmp->op != PIPE)
        tmp = tmp->next;

    if (tmp && tmp->op == PIPE)
    {
        right_tokens = tmp->next;
        tmp->next = NULL;
    }

    if (pipe(fd) == -1)
    {
        perror("pipe");
        return;
    }

    pid1 = fork();
    if (pid1 == 0)
        execute_left_side(left_tokens, glb, fd, exit_status);

    pid2 = fork();
    if (pid2 == 0)
        execute_right_side(right_tokens, glb, fd, exit_status);

    signal_handler();
    close(fd[READING_END]);
    close(fd[WRITING_END]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, &status, 0);

    extract_exit_status(status, exit_status);
    save_exit_status(glb, *exit_status);
}
