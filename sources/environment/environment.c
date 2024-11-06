/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 17:58:31 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/11/06 10:49:47 by mrahmat-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*ft_envnew(char *key, char *value)
{
	t_env	*node;

	if (key == NULL || value == NULL)
		return (NULL);
	node = (t_env *)malloc(sizeof(t_env));
	if (node == NULL)
		return (NULL);
	node->key = key;
	node->value = value;
	if (value == NULL || *value == '\0')
		node->flag = false;
	else
		node->flag = true;
	node->next = NULL;
	return (node);
}

void	ft_envadd_back(t_env **lst, t_env *new)
{
	t_env	*last;

	if (lst == NULL || new == NULL)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	last = *lst;
	while (last->next != NULL)
		last = last->next;
	last->next = new;
}

void	ft_envadd_front(t_env **lst, t_env *new)
{
	if (new == NULL)
		return ;
	new->next = *lst;
	*lst = new;
}

void	ft_envdelone(t_env *lst, void (*del)(void *))
{
	if (lst == NULL || del == NULL)
		return ;
	del(lst->key);
	del(lst->value);
	free(lst);
}

void	ft_envclear(t_env **lst, void (*del)(void *))
{
	t_env	*node;
	t_env	*temp;

	if (lst == NULL || del == NULL)
		return ;
	node = *lst;
	while (node != NULL)
	{
		temp = node->next;
		del(node->key);
		del(node->value);
		free(node);
		node = temp;
	}
	*lst = NULL;
}
