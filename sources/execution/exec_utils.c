/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 17:23:18 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/10/24 17:30:50 by mrahmat-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*copy_env_node(t_env *env)
{
	char	*node_cpy;
	char	*temp;

	node_cpy = ft_strdup(env->key);
	if (node_cpy == NULL)
		return (NULL);
	temp = node_cpy;
	node_cpy = ft_strjoin(node_cpy, "=");
	free(temp);
	if (node_cpy == NULL)
		return (NULL);
	temp = node_cpy;
	node_cpy = ft_strjoin(node_cpy, env->value);
	free(temp);
	if (node_cpy == NULL)
		return (NULL);
	return (node_cpy);
}
