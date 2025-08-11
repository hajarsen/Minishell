#include "../includes/minishell.h"

void	*gc_alloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	ft_lstadd_back(&glb_list()->gc, ft_lstnew(ptr));
	return (ptr);
}

void	gc_free_all(void)
{
	t_list	*tmp;

	while (glb_list()->gc)
	{
		tmp = glb_list()->gc->next;
		free(glb_list()->gc->content);
		free(glb_list()->gc);
		glb_list()->gc = tmp;
	}
}
