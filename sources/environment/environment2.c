/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 10:55:16 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/11/06 11:03:52 by mrahmat-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	**update_shlvl(t_env **node)
{
	int	shlvl;

	shlvl = ft_atoi((*node)->value);
	if (shlvl <= 0)
	{
		ft_envclear(node, &free);
		return (NULL);
	}
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

t_env	*copy_env(char **envp)
{
	t_env	*env;
	t_env	*new_node;

	env = NULL;
	while (*envp != NULL)
	{
		new_node = set_key_value(*envp);
		if (new_node == NULL)
		{
			ft_envclear(&env, &free);
			return (NULL);
		}
		if (ft_strcmp(new_node->key, "SHLVL") == 0)
		{
			update_shlvl(&new_node);
			if (new_node == NULL)
			{
				ft_envclear(&env, &free);
				return (NULL);
			}
		}
		ft_envadd_back(&env, new_node);
		envp++;
	}
	return (env);
}
