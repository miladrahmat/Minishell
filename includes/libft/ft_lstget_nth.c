/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstget_nth.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:00:40 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/18 17:22:48 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// index starts at zero
t_list	*ft_lstget_nth(t_list *lst, int n)
{
	if (!lst)
		return (NULL);
	while (n > 0)
	{
		if (!lst->next)
			return (NULL);
		lst = lst->next;
		n--;
	}
	return (lst);
}
