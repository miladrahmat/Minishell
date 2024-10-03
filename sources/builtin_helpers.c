/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 18:32:17 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/10/03 19:20:30 by mrahmat-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	connect_list(t_list **list, t_list **node)
{
	t_list	*node_before;
	t_list	*node_after;

	node_before = *list;
	while (node_before->next != *node && node_before != NULL)
		node_before = node_before->next;
	node_after = (*node)->next;
	node_before->next = node_after;
	ft_lstdelone(*node, &free);
	*node = node_before->next;
}

void	add_node_to_index(t_list **list, t_list **node)