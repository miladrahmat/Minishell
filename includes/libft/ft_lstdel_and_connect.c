/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdel_and_connect.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 11:59:45 by lemercie          #+#    #+#             */
/*   Updated: 2024/12/05 17:51:02 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdel_and_connect(t_list **head, t_list **to_del)
{
	t_list	*temp;

	if (!*head || !*to_del)
		return ;
	if (!(*head)->next)
	{
		ft_lstdelone(*head, &free);
		*head = NULL;
		*to_del = *head;
		return ;
	}
	temp = *head;
	if (*head == *to_del)
	{
		*head = temp->next;
		*to_del = temp->next;
		ft_lstdelone(temp, &free);
		return ;
	}
	while (temp != NULL && temp->next != *to_del)
		temp = temp->next;
	temp->next = (*to_del)->next;
	ft_lstdelone(*to_del, &free);
	*to_del = temp;
}
