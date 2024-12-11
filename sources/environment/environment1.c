/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 18:20:21 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/12/11 16:50:08 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_envsize(t_env *env)
{
	int	size;

	size = 0;
	while (env != NULL)
	{
		env = env->next;
		size++;
	}
	return (size);
}

void	ft_env_free_add(t_env *lst, char *key, char *value)
{
	if (lst == NULL)
		return ;
	free(lst->key);
	free(lst->value);
	lst->key = key;
	lst->value = value;
	if (*value == '\0')
		lst->flag = false;
	else
		lst->flag = true;
}

t_env	*ft_envcpy(t_env *envp)
{
	t_env	*new_env;
	t_env	*new_node;
	char	*key;
	char	*value;

	new_env = NULL;
	while (envp != NULL)
	{
		key = ft_strdup(envp->key);
		value = ft_strdup(envp->value);
		if (value == NULL || key == NULL)
		{
			ft_envclear(&new_env, &free);
			if (key)
				free(key);
			return (NULL);
		}
		new_node = ft_envnew(key, value);
		new_node->flag = envp->flag;
		ft_envadd_back(&new_env, new_node);
		envp = envp->next;
	}
	return (new_env);
}

char	*ft_env_get_value_by_key(char *key, t_env *env)
{
	while (env)
	{
		if (ft_strcmp(key, env->key) == 0)
		{
			return (env->value);
		}
		env = env->next;
	}
	return (NULL);
}
