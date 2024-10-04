/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 18:20:21 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/10/04 19:09:58 by mrahmat-         ###   ########.fr       */
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

t_env	*get_key_value(char *str)
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
