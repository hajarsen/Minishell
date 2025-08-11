#include "minishell.h"

int update_env_var(t_env *env_list, t_env *new_node)
{
    t_env *tmp = env_list;
    char *new_value;

    while (tmp)
    {
        if (ft_strcmp(tmp->name, new_node->name) == 0)
        {
            if (!new_node->value)
                return 1;
            new_value = ft_strdup(new_node->value);
            if (!new_value)
                return 1;
            free(tmp->value);
            tmp->value = new_value;
            return 1;
        }
        tmp = tmp->next;
    }
    return 0;
}
void update_shell_lvl(t_env **env_list)
{
    char *shell_lvl = get_env_value("SHLVL", *env_list);
    int lvl = 1;
    char *new_lvl_str;

    if (shell_lvl)
        lvl = ft_atoi(shell_lvl) + 1;
    new_lvl_str = ft_itoa(lvl);
    if (!new_lvl_str)
        return;
    insert_env_node(env_list, create_env_node(ft_strdup("SHLVL"), new_lvl_str));
}
