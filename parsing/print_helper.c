#include "minishell.h"

void	print_tokenizer(t_tokenizer *tokens)
{
	int i = 0;
	t_tokenizer	*temp;
	t_here_doc	*tmp;

	temp = tokens;
	while (temp != NULL)
	{
		printf("index : %d\n",temp->i);
		if (temp->op == NOT_OP)
			printf("token   : %s = %zu\n\n\n",temp->str, ft_strlen(temp->str));
		else
		{
			printf("OP      : ");
			print_op(temp->op);
			printf("\n");
			if (temp->op == LESS_LESS)
			{
				tmp = temp->hd;
				printf("\n************HERE IS THE HEREDOC**************\n");
				while (tmp != 0)
				{
					printf("%s \n", tmp->str);
					tmp = tmp->next;
				}
			}
		}
		temp = temp->next;
		i++;
	}
	printf("===================================================================\n");
}

void	print_tree(t_ast	*ast)
{
	while (ast != NULL)
	{
		if (ast->type != PIPE)
		{
			print_tokenizer(ast->cmd_line);
		}
		if (ast->left != NULL && ast->left->type != PIPE)
		{
			print_tokenizer(ast->left->cmd_line);
		}
		ast = ast->left;
	}
}

void	print_node(t_ast	*ast)
{
	if (ast->type == PIPE)
		printf("\n_________PIPE____________\n");
	print_tokenizer(ast->cmd_line);
}

void	print_op(t_operator op)
{
	if (op == LESS_LESS)
		printf("<<\n");
	else if (op == GREAT_GREAT)
		printf(">>\n");
	else if (op == PIPE)
		printf("|\n");
	else if (op == LESS)
		printf("<\n");
	else if (op == GREAT)
		printf(">\n");
	else if (op == DOUBLE_QUOTE)
		printf("\"\n");
	else if (op == QUOTE)
		printf("\'\n");
	else
		printf("something else");
}

void	print_env(t_env *env_list)
{
	while(env_list != NULL)
	{
		printf("%s", env_list->name);
		printf("=");
		printf("%s %zu\n", env_list->value, ft_strlen(env_list->value));
		env_list = env_list->next;
	}
}
