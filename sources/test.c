/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:35:34 by lemercie          #+#    #+#             */
/*   Updated: 2024/10/07 14:26:15 by lemercie         ###   ########.fr       */
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
			cmd_table = init_cmd_table(tokens);
		//	ft_lstiter(cmd_table, &print_list);
			printf("%i - ", ((t_cmd *)cmd_table->content)->infile);
			printf("%i\n", ((t_cmd *)cmd_table->content)->outfile);
			i = 0;
			while (((t_cmd *)cmd_table->content)->cmd[1])
			{
				printf("%sOMG\n", ((t_cmd *)cmd_table->content)->cmd[i]);
				i++;
			}
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

