#include "minishell.h"

t_glb	*glb_list(void)
{
	static t_glb	glb;

	return (&glb);
}
