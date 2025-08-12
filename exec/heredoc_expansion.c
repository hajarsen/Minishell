#include "minishell.h"

static int is_name_char(int c)
{
    return (ft_isalnum(c) || c == '_');
}

int heredoc_delimiter_is_quoted(t_tokenizer *delim_tok)
{
    if (!delim_tok)
        return (0);
    return (delim_tok->redirect.qt == THERES_QUOTE);
}

static char *expand_var(const char *p, size_t *consumed, t_env *env)
{
    char *name;
    char *val;
    char *ret;

    if (p[1] == '?')
    {
        *consumed = 2;
        return (ft_itoa(glb_list()->exit_status));
    }
    if (ft_isdigit((unsigned char)p[1]))
    {
        *consumed = 2;
        return (ft_strdup(""));
    }
    if (!is_name_char((unsigned char)p[1]))
    {
        *consumed = 1;
        return (ft_strdup("$"));
    }
    {
        size_t i = 1;
        while (is_name_char((unsigned char)p[i]))
            i++;
        name = ft_substr(p, 1, i - 1);
        val = get_env_value(name, env);
        free(name);
        ret = ft_strdup(val ? val : "");
        *consumed = i;
        return (ret);
    }
}

char *expand_heredoc_line(const char *line, t_env *env)
{
    size_t i = 0;
    char *out = ft_strdup("");
    while (line && line[i])
    {
        if (line[i] == '$')
        {
            size_t used = 0;
            char *frag = expand_var(line + i, &used, env);
            char *tmp = ft_strjoin(out, frag);
            free(out);
            free(frag);
            out = tmp;
            i += used;
            continue;
        }
        {
            char buf[2] = { (char)line[i], 0 };
            char *tmp = ft_strjoin(out, buf);
            free(out);
            out = tmp;
            i++;
        }
    }
    return (out);
}
