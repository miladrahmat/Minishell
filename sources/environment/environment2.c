/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 10:55:16 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/11/15 15:23:28 by mrahmat-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update__(char **cmd, t_env **env)
{
	t_env	*env_iter;
	char	*temp;

	if (cmd == NULL)
		return ;
	if (*cmd == NULL)
		return ;
	env_iter = *env;
	while (env_iter != NULL)
	{
		if (ft_strcmp("_", env_iter->key) == 0)
			break ;
		else
			env_iter = env_iter->next;
	}
	if (env_iter == NULL)
		return ;
	temp = ft_strdup(*cmd);
	if (temp == NULL)
		return ;
	free(env_iter->value);
	env_iter->value = temp;
}

void	**update_shlvl(t_env **node)
{
	int	shlvl;

	shlvl = ft_atoi((*node)->value);
	if (shlvl <= 0 || shlvl == INT_MAX)
		shlvl = 0;
	else
		shlvl++;
	free((*node)->value);
	(*node)->value = ft_itoa(shlvl);
	if ((*node)->value == NULL)
	{
		ft_envclear(node, &free);
		return (NULL);
	}
	return ((void **)node);
}

t_env	*copy_env(char **envp, int *err)
{
	t_env	*env;
	t_env	*new_node;

	env = NULL;
	while (*envp != NULL)
	{
		new_node = set_key_value(*envp);
		if (new_node == NULL)
		{
			*err = 1;
			return (env);
		}
		if (ft_strcmp(new_node->key, "SHLVL") == 0)
		{
			update_shlvl(&new_node);
			if (new_node == NULL)
			{
				*err = 1;
				return (env);
			}
		}
		ft_envadd_back(&env, new_node);
		envp++;
	}
	return (env);
}
