/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 18:32:17 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/10/10 17:41:30 by mrahmat-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	connect_list(t_env **list, t_env **node)
{
	t_env	*node_before;
	t_env	*node_after;

	node_before = *list;
	if (node_before == *node)
	{
		node_before = node_before->next;
		ft_envdelone(*node, &free);
		*list = node_before;
		*node = node_before;
		return ;
	}
	while (node_before->next != *node && node_before != NULL)
		node_before = node_before->next;
	node_after = (*node)->next;
	node_before->next = node_after;
	ft_envdelone(*node, &free);
	*node = node_before;
}

size_t	ft_strlen_eq(char *str)
{
	size_t	eq;

	eq = 0;
	while (str[eq] != '=' && str[eq] != '\0')
		eq++;
	return (eq);
}

bool	check_key(char *cmd, t_env *node)
{
	size_t	occ_cmd;
	bool	match;

	occ_cmd = 0;
	while (cmd[occ_cmd] != '=' && cmd[occ_cmd] != '\0')
		occ_cmd++;
	if (occ_cmd != ft_strlen(node->key))
		return (true);
	if (ft_strncmp(cmd, node->key, ft_strlen(node->key)) == 0)
		match = false;
	else
		match = true;
	return (match);
}

size_t	get_cmd_amount(char **cmd)
{
	size_t	amount;

	amount = 0;
	while (*(cmd + amount) != NULL)
		amount++;
	return (amount);
}
