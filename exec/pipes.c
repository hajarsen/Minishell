#include "minishell.h"

static void ignore_interactive_signals(void)
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

static int count_segments(t_tokenizer *tokens)
{
    int count = 0;
    t_tokenizer *t = tokens;
    while (t)
    {
        count++;
        while (t && t->op != PIPE)
            t = t->next;
        if (t && t->op == PIPE)
            t = t->next;
    }
    return count;
}

static void collect_segments(t_tokenizer *tokens, t_tokenizer **starts, t_tokenizer **ends, int n)
{
    int i = 0;
    t_tokenizer *t = tokens;
    while (t && i < n)
    {
        t_tokenizer *prev = NULL;
        starts[i] = t;
        while (t && t->op != PIPE)
        {
            prev = t;
            t = t->next;
        }
        ends[i] = prev;
        if (t && t->op == PIPE)
            t = t->next;
        i++;
    }
}

static void close_all_pipes(int (*pfds)[2], int n)
{
    int i = 0;
    while (i < n)
    {
        close(pfds[i][READING_END]);
        close(pfds[i][WRITING_END]);
        i++;
    }
}

static void child_exec_segment(int idx, int nseg, int (*pfds)[2], t_tokenizer *seg_head, t_tokenizer *seg_tail, t_glb *glb, int *exit_status)
{
    int j;

    if (seg_tail)
        seg_tail->next = NULL; 

    if (idx > 0)
    {
        if (dup2(pfds[idx - 1][READING_END], STDIN_FILENO) < 0)
        {
            perror("dup2");
            exit(1);
        }
    }
    if (idx < nseg - 1)
    {
        if (dup2(pfds[idx][WRITING_END], STDOUT_FILENO) < 0)
        {
            perror("dup2");
            exit(1);
        }
    }

    j = 0;
    while (j < nseg - 1)
    {
        close(pfds[j][READING_END]);
        close(pfds[j][WRITING_END]);
        j++;
    }

    if (execute_redirections(seg_head))
        exit(1);

    char **args = tokens_to_args(seg_head);
    if (!args || !args[0])
        exit(0);

    if (is_builtin(args[0]))
    {
        int is_blt = execute_builtin(args, &glb->env, exit_status);
        if (is_blt)
            exit(*exit_status);
    }

    char *path = get_cmd_path(args[0], glb->env);
    if (!path)
    {
        ft_putstr_fd("minishell: command not found: ", 2);
        ft_putendl_fd(args[0], 2);
        exit(127);
    }

    struct stat path_stat;
    if (stat(path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(args[0], 2);
        ft_putstr_fd(": Is a directory\n", 2);
        free(path);
        exit(126);
    }

    char **envp = envlist_to_array(glb->env);
    if (!envp)
    {
        perror("envlist_to_array");
        free(path);
        exit(1);
    }

    execve(path, args, envp);
    {
        int saved_errno = errno;
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(args[0], 2);
        ft_putstr_fd(": ", 2);
        ft_putendl_fd(strerror(saved_errno), 2);
        free(path);
        free_strs(envp);
        if (saved_errno == ENOENT)
            exit(127);
        else if (saved_errno == EACCES || saved_errno == ENOTDIR || saved_errno == EISDIR || saved_errno == ENOEXEC)
            exit(126);
        else
            exit(126);
    }
}

void execute_pipeline(t_tokenizer *tokens, t_glb *glb, int *exit_status)
{
    int nseg = count_segments(tokens);
    if (nseg <= 0)
        return;

    t_tokenizer **starts = gc_alloc(sizeof(t_tokenizer *) * nseg);
    t_tokenizer **ends = gc_alloc(sizeof(t_tokenizer *) * nseg);
    if (!starts || !ends)
        return;
    collect_segments(tokens, starts, ends, nseg);

    int (*pfds)[2] = NULL;
    if (nseg > 1)
    {
        pfds = malloc(sizeof(int[2]) * (nseg - 1));
        if (!pfds)
        {
            perror("malloc");
            return;
        }
        for (int i = 0; i < nseg - 1; i++)
        {
            if (pipe(pfds[i]) == -1)
            {
                perror("pipe");
                for (int k = 0; k < i; k++)
                {
                    close(pfds[k][READING_END]);
                    close(pfds[k][WRITING_END]);
                }
                free(pfds);
                return;
            }
        }
    }

    pid_t *pids = malloc(sizeof(pid_t) * nseg);
    if (!pids)
    {
        perror("malloc");
        if (pfds)
        {
            close_all_pipes(pfds, nseg - 1);
            free(pfds);
        }
        return;
    }

    for (int i = 0; i < nseg; i++)
    {
        pid_t pid = fork();
        if (pid < 0)
        {
            perror("fork");
            pids[i] = -1;
            continue;
        }
        if (pid == 0)
        {
            if (nseg > 1)
                child_exec_segment(i, nseg, pfds, starts[i], ends[i], glb, exit_status);
            else
            {
                if (execute_redirections(starts[i]))
                    exit(1);
                char **args = tokens_to_args(starts[i]);
                if (!args || !args[0])
                    exit(0);
                if (is_builtin(args[0]))
                {
                    int is_blt = execute_builtin(args, &glb->env, exit_status);
                    if (is_blt)
                        exit(*exit_status);
                }
                char *path = get_cmd_path(args[0], glb->env);
                if (!path)
                {
                    ft_putstr_fd("minishell: command not found: ", 2);
                    ft_putendl_fd(args[0], 2);
                    exit(127);
                }
                struct stat st;
                if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
                {
                    ft_putstr_fd("minishell: ", 2);
                    ft_putstr_fd(args[0], 2);
                    ft_putstr_fd(": Is a directory\n", 2);
                    free(path);
                    exit(126);
                }
                char **envp = envlist_to_array(glb->env);
                if (!envp)
                {
                    perror("envlist_to_array");
                    free(path);
                    exit(1);
                }
                execve(path, args, envp);
                {
                    int saved_errno = errno;
                    ft_putstr_fd("minishell: ", 2);
                    ft_putstr_fd(args[0], 2);
                    ft_putstr_fd(": ", 2);
                    ft_putendl_fd(strerror(saved_errno), 2);
                    free(path);
                    free_strs(envp);
                    if (saved_errno == ENOENT)
                        exit(127);
                    else if (saved_errno == EACCES || saved_errno == ENOTDIR || saved_errno == EISDIR || saved_errno == ENOEXEC)
                        exit(126);
                    else
                        exit(126);
                }
            }
        }
        pids[i] = pid;
    }

    // Parent 
    ignore_interactive_signals();
    if (pfds)
    {
        close_all_pipes(pfds, nseg - 1);
        free(pfds);
    }

    int status = 0;
    int last_status = 0;
    for (int i = 0; i < nseg; i++)
    {
        if (pids[i] > 0)
        {
            waitpid(pids[i], &status, 0);
            if (i == nseg - 1)
                last_status = status;
        }
    }
    free(pids);

    extract_exit_status(last_status, exit_status);
    save_exit_status(glb, *exit_status);
}
