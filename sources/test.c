/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:35:34 by lemercie          #+#    #+#             */
/*   Updated: 2024/10/09 11:32:58 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	split_free(char **str)
{
	size_t	i;

	i = 0;
	while (str[i] != NULL)
	{
		free(str[i]);
		i++;
	}
	free(str[i]);
	free(str);
}

t_list	*copy_env(char **envp)
{
	t_list	*env;
	t_list	*new_node;
	char	*copy;

	env = NULL;
	while (*envp != NULL)
	{
		copy = ft_strdup(*envp);
		if (copy == NULL)
			ft_lstclear(&env, &free);
		new_node = ft_lstnew(copy);
		ft_lstadd_back(&env, new_node);
		envp++;
	}
	return (env);
}

void	print_list(void *arg)
{
	printf("%s\n", (char *) arg);
}

int	main(int ac, char **av, char **envp)
{
	char	*line;
	char	**cmd;
	t_list	*env;
	t_list	*tokens;
	t_list	*cmd_table;
	t_list	*cmd_table_iter;
	int		i;

	(void)av;
	(void)ac;
	env = copy_env(envp);
	while (true)
	{
		line = readline("\e[1;32m[MINISHELL]$> \e[0m");
		if (line && *line)
		{
			tokens = tokenize(line);
			ft_lstiter(tokens, &print_list);
			cmd_table = init_cmd_table(tokens, envp);
		//	ft_lstiter(cmd_table, &print_list);
			cmd_table_iter = cmd_table;
			while (cmd_table_iter)
			{
				printf("infile: %i\n", ((t_cmd *)cmd_table_iter->content)->infile);
				printf("outfile: %i\n", ((t_cmd *)cmd_table_iter->content)->outfile);
				i = 0;
				while (((t_cmd *)cmd_table_iter->content)->cmd &&
					((t_cmd *)cmd_table_iter->content)->cmd[i])
				{
					printf("cmd: %s\n", ((t_cmd *)cmd_table_iter->content)->cmd[i]);
					i++;
				}
				cmd_table_iter = cmd_table_iter->next;
			}
			printf("done\n");
			cmd = ft_split(line, ' ');
			add_history(line);
			free(line);
			if (cmd == NULL || *cmd == NULL)
			{
				ft_lstclear(&env, &free);
				exit(1);
			}
			check_builtin_cmd(cmd, 1, &env);
			split_free(cmd);
		}
	}
}
