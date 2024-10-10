/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:35:34 by lemercie          #+#    #+#             */
/*   Updated: 2024/10/10 18:56:47 by mrahmat-         ###   ########.fr       */
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
	t_env	*env;

	(void)av;
	(void)ac;
	env = copy_env(envp);
	while (true)
	{
		line = readline("\e[1;32m[MINISHELL]$> \e[0m");
		if (line && *line)
		{
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
