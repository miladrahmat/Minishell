/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdel_and_connect.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 11:59:45 by lemercie          #+#    #+#             */
/*   Updated: 2024/12/03 17:12:06 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdel_and_connect(t_list **head, t_list **node)
{
	t_list	*node_before;
	t_list	*node_after;

	if (!(*head)->next)
	{
		ft_lstdelone(*head, &free);
		*head = NULL;
		*node = *head;
		return ;
	}
	node_before = *head;
	if (node_before == *node)
	{
		node_before = node_before->next;
		ft_lstdelone(*node, &free);
		*head = node_before;
		*node = node_before;
		return ;
	}
	while (node_before != NULL && node_before->next != *node)
		node_before = node_before->next;
	node_after = (*node)->next;
	node_before->next = node_after;
	ft_lstdelone(*node, &free);
	*node = node_after;
}
