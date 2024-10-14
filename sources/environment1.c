/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 18:20:21 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/10/14 11:33:56 by mrahmat-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

t_env	*set_key_value(char *str)
{
	t_env	*node;
	char	*key;
	char	*value;
	size_t	cut;

	cut = ft_strlen_eq(str);
	key = ft_substr(str, 0, cut);
	if (key == NULL)
		return (NULL);
	value = ft_substr(str, cut + 1, ft_strlen(str) - cut);
	if (value == NULL)
	{
		free(key);
		return (NULL);
	}
	node = ft_envnew(key, value);
	return (node);
}

t_env	*ft_envcpy(t_env *envp)
{
	t_env	*new_env;
	char	*key;
	char	*value;

	new_env = NULL;
	while (envp != NULL)
	{
		key = ft_strdup(envp->key);
		if (key == NULL)
		{
			ft_envclear(&new_env, &free);
			return (NULL);
		}
		value = ft_strdup(envp->value);
		if (value == NULL)
		{
			ft_envclear(&new_env, &free);
			free(key);
			return (NULL);
		}
		ft_envadd_back(&new_env, ft_envnew(key, value));
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
