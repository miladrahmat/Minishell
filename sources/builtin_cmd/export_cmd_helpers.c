/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_cmd_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 15:56:15 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/29 15:56:43 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	export_key(char *cmd, size_t cut, t_env **envp)
{
	char	*key;
	char	*value;

	key = ft_substr(cmd, 0, cut);
	value = ft_substr(cmd, cut + 1, ft_strlen(cmd) - cut);
	if (value == NULL || key == NULL)
	{
		free_strs(&value, &key);
		return (false);
	}
	ft_env_free_add(*envp, key, value);
	if (cmd[cut] == '\0')
		(*envp)->flag = false;
	else
		(*envp)->flag = true;
	return (true);
}

bool	export_existing_key(char *cmd, t_env **envp)
{
	t_env	*list_iter;
//	char	*key;
//	char	*value;
	size_t	cut;

	list_iter = *envp;
	cut = ft_strlen_eq(cmd);
	while (list_iter != NULL)
	{
		if (ft_strncmp(cmd, list_iter->key, ft_strlen(list_iter->key)) == 0)
		{
			return (export_key(cmd, cut, &list_iter));
			/*
			key = ft_substr(cmd, 0, cut);
			value = ft_substr(cmd, cut + 1, ft_strlen(cmd) - cut);
			if (value == NULL || key == NULL)
			{
				free_strs(&value, &key);
				return (false);
			}
			ft_env_free_add(list_iter, key, value);
			if (cmd[cut] == '\0')
				list_iter->flag = false;
			else
				list_iter->flag = true;
			return (true);
			*/
		}
		list_iter = list_iter->next;
	}
	return (false);
}
