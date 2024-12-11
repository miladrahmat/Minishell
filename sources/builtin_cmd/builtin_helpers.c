/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 18:32:17 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/12/11 14:46:58 by mrahmat-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_pwd(t_env **envp)
{
	t_env	*list_iter;
	char	*res;
	char	*old_pwd;

	res = get_pwd();
	if (res == NULL)
		return ;
	old_pwd = NULL;
	list_iter = *envp;
	while (list_iter != NULL)
	{
		if (ft_strcmp(list_iter->key, "PWD") == 0 && old_pwd == NULL)
		{
			old_pwd = list_iter->value;
			list_iter->value = res;
			list_iter = *envp;
		}
		if (ft_strcmp(list_iter->key, "OLDPWD") == 0 && old_pwd != NULL)
		{
			free(list_iter->value);
			list_iter->value = old_pwd;
		}
		list_iter = list_iter->next;
	}
}

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

size_t	get_cmd_amount(char **cmd)
{
	size_t	amount;

	amount = 0;
	while (*(cmd + amount) != NULL)
		amount++;
	return (amount);
}

char	*get_pwd(void)
{
	char	*res;

	res = malloc(PATH_MAX * sizeof(char));
	if (res == NULL)
		return (NULL);
	if (getcwd(res, PATH_MAX) == NULL)
	{
		free(res);
		return (NULL);
	}
	return (res);
}
