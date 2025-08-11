#include "minishell.h"

/*This function save the env variables in a varibale (act as global var) glb_list()->env */
// t_env	*save_env(char **env)
// {
// 	t_env	*env_list;
// 	t_env	*env_head;
// 	int		i;
// 	int		j;

// 	i = 0;
// 	env_head = malloc(sizeof(t_env));
// 	env_list = env_head;
// 	while (env[i] != 0)	
// 	{
// 		j = 0;
// 		while (env[i][j] != '=')
// 			j++;
// 		env_list->name = ft_substr(env[i], 0, j);
// 		env_list->value = ft_substr(env[i], j + 1, ft_strlen(env[i]));
// 		i++;
// 		if (env[i] != NULL)
// 			env_list->next = malloc(sizeof(t_env));
// 		else
// 			break;
// 		env_list = env_list->next;
// 	}
// 	env_list->next = NULL;
// 	return (env_head);
// }

int	main(int ac, char **av, char **env)
{
	t_tokenizer *tokens;
	char		*input;

	(void)ac;
	(void)av;
	//signal_handler();
	glb_list()->env = save_env(env);
	while (1)
	{
		input = readline("\033[1;32m➜\033[0m\033[1;36m Minishell@damn:$ \033[0m");
		if (input_error(input) == 1)
			break;
		tokens = tokenizer(input);
		if (check_parsing_errors(tokens))
			return (1);
		expanding(&tokens);
		printf("this is the main tokenizer :");
		print_tokenizer(tokens);
		free_tokens(input, tokens);
	}
	free_env(glb_list()->env);
}
		// t_ast		*ast;
		// ast = ast_builder(tokens);
		// printf("________________NO_THE_TREE_________________");
		// print_node(ast);
		// printf("this is the right \n");
		// print_node(ast->left);
		// printf("this is the left \n");
		// print_node(ast->left);
