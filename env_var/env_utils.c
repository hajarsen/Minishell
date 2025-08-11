#include "minishell.h"

int ft_strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return ((unsigned char)*s1 - (unsigned char)*s2);
}


t_env *create_env_node(char *name, char *value)
{
    t_env *new;

    new = gc_alloc(sizeof(t_env));
    if (!new)
        return NULL;
    new->name = name;
    new->value = value;
    new->next = NULL;
    return new;
}
void insert_env_node(t_env **env_list, t_env *new_node)
{
    if (update_env_var(*env_list, new_node))
        return;
    if (!*env_list || ft_strcmp(new_node->name, (*env_list)->name) < 0)
    {
        new_node->next = *env_list;
        *env_list = new_node;
        return;
    }
    insert_env_node(&((*env_list)->next), new_node);
}
char *get_env_value(char *name, t_env *env)
{
    t_env *tmp = env;

    while (tmp)
    {
        if (ft_strcmp(tmp->name, name) == 0)
            return tmp->value;
        tmp = tmp->next;
    }
    return NULL;
}
void free_env(t_env *env)
{
    t_env *tmp;

    while (env)
    {
        tmp = env->next;
        free(env->name);
        free(env->value);
        free(env);
        env = tmp;
    }
}
