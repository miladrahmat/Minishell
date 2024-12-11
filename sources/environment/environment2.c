/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 10:55:16 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/12/11 14:32:23 by mrahmat-         ###   ########.fr       */
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

int	add_new_env(char *new_variable, t_env **envp)
{
	t_env	*new_node;
	size_t	cut;

	cut = ft_strlen_eq(new_variable);
	if (cut == 0 && new_variable[cut] == '\0')
		return (-1);
	else if (cut == 0 && new_variable[cut] == '=')
	{
		print_export_error(&new_variable, false);
		return (-1);
	}
	new_node = set_key_value(new_variable);
	if (new_node == NULL)
		return (-1);
	if (new_variable[cut] == '\0')
		new_node->flag = false;
	else
		new_node->flag = true;
	ft_envadd_back(envp, new_node);
	return (1);
}
