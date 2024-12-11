/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_cmd_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 15:56:15 by lemercie          #+#    #+#             */
/*   Updated: 2024/12/11 16:47:48 by lemercie         ###   ########.fr       */
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
	size_t	cut;
	char	*temp_key;

	list_iter = *envp;
	cut = ft_strlen_eq(cmd);
	temp_key = ft_substr(cmd, 0, cut);
	if (temp_key == NULL)
		return (false);
	while (list_iter != NULL)
	{
		if (ft_strcmp(temp_key, list_iter->key) == 0)
		{
			free(temp_key);
			return (export_key(cmd, cut, &list_iter));
		}
		list_iter = list_iter->next;
	}
	free(temp_key);
	return (false);
}
