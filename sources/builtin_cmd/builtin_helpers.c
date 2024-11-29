/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 18:32:17 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/11/29 12:27:31 by mrahmat-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_pwd(t_env **envp)
{
	t_env	*list_iter;
	char	*res;
	char	*old_pwd;

	res = malloc(PATH_MAX * sizeof(char));
	if (res == NULL)
		return ;
	getcwd(res, PATH_MAX);
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

int	add_new_env(char *new_variable, t_env **envp)
{
	t_env	*new_node;
	size_t	cut;

	cut = ft_strlen_eq(new_variable);
	if (cut == 0 && new_variable[cut] == '\0')
		return (-1);
	else if (cut == 0 && new_variable[cut] == '=')
	{
		print_export_error(&new_variable, false);
		return (-1);
	}
	new_node = set_key_value(new_variable);
	if (new_node == NULL)
		return (-1);
	if (new_variable[cut] == '\0')
		new_node->flag = false;
	else
		new_node->flag = true;
	ft_envadd_back(envp, new_node);
	return (1);
}

size_t	get_cmd_amount(char **cmd)
{
	size_t	amount;

	amount = 0;
	while (*(cmd + amount) != NULL)
		amount++;
	return (amount);
}
