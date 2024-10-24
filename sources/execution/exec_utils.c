/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 17:23:18 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/10/24 18:12:46 by mrahmat-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_one_builtin(t_list *cmd_table, t_env **env)
{
	if (ft_lstsize(cmd_table) == 1)
		return (check_builtin_cmd(((t_cmd *)cmd_table->content)->cmd_args, \
			((t_files *)((t_cmd *)cmd_table->content)->fd)->outfile, env));
	return (-1);
}

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
