/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:35:34 by lemercie          #+#    #+#             */
/*   Updated: 2024/10/17 14:05:48 by lemercie         ###   ########.fr       */
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
	free(str[i]); //why is this line here?
	free(str);
}

t_env	*copy_env(char **envp)
{
	t_env	*env;
	t_env	*new_node;

	env = NULL;
	while (*envp != NULL)
	{
		new_node = get_key_value(*envp);
		ft_envadd_back(&env, new_node);
		envp++;
	}
	return (env);
}


int	main(int ac, char **av, char **envp)
{
	char	*line;
	char	**cmd;
	t_list	*cmd_table;
	t_list	*cmd_table_iter;
	int		i;
	t_env	*env;

	(void)av;
	(void)ac;
	env = copy_env(envp);
	while (true)
	{
		line = readline("\e[1;32m[MINISHELL]$> \e[0m");
		if (line && *line)
		{
			cmd_table = init_cmd_table(line, env);
			printf("test.c: cmd table initialized\n");
			cmd_table_iter = cmd_table;
			while (cmd_table_iter)
			{
				if (((t_cmd *)cmd_table_iter->content)->infiles)
					printf("infile: %s\n", ((t_redir *)((t_cmd *)
						cmd_table_iter->content)->infiles->content)->filename);
				if (((t_cmd *)cmd_table_iter->content)->outfiles)
					printf("outfile: %s\n", ((t_redir *)((t_cmd *)
						cmd_table_iter->content)->outfiles->content)->filename);
				i = 0;
				while (((t_cmd *)cmd_table_iter->content)->cmd_args &&
					((t_cmd *)cmd_table_iter->content)->cmd_args[i])
				{
					printf("cmd: %s\n",
						((t_cmd *)cmd_table_iter->content)->cmd_args[i]);
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
				ft_envclear(&env, &free);
				exit(1);
			}
			check_builtin_cmd(cmd, 1, &env);
			split_free(cmd);
		}
	}
}
