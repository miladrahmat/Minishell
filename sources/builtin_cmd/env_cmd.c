/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 11:02:40 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/11/13 15:06:49 by mrahmat-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env(char **cmd, int fd, t_env **envp)
{
	t_env	*list_iter;

	if (cmd[1] != NULL)
	{
		print_error("env", "Too many arguments");
		return (0);
	}
	list_iter = *envp;
	while (list_iter != NULL)
	{
		if (list_iter->flag == true)
		{
			ft_putstr_fd(list_iter->key, fd);
			ft_putchar_fd('=', fd);
			ft_putendl_fd(list_iter->value, fd);
		}
		list_iter = list_iter->next;
	}
	return (0);
}

int	unset(char **cmd, t_env **envp)
{
	t_env	*list_iter;
	size_t	cmd_iter;

	list_iter = *envp;
	while (list_iter != NULL)
	{
		cmd_iter = 0;
		while (cmd_iter < get_cmd_amount(cmd))
		{
			if (ft_strncmp(*(cmd + cmd_iter), list_iter->key, \
				ft_strlen(list_iter->key)) == 0)
				connect_list(envp, &list_iter);
			cmd_iter++;
		}
		if (list_iter != NULL)
			list_iter = list_iter->next;
	}
	return (0);
}
