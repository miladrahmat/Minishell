/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 11:02:40 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/10/02 16:46:26 by mrahmat-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env(char **cmd, int fd, t_list *envp)
{
	t_list	*list_iter;

	if (cmd[1] != NULL)
	{
		print_error("env", "Too many arguments");
		return (0);
	}
	list_iter = envp;
	while (list_iter != NULL)
	{
		ft_putendl_fd((char *)list_iter->content, fd);
		list_iter = list_iter->next;
	}
	return (0);
}

int	unset(char **cmd, t_list *envp)
{
	t_list	*new_list;
	t_list	*new_node;
	t_list	*list_iter;

	new_list = NULL;
	list_iter = envp;
	while (list_iter != NULL)
	{
		if (ft_strncmp((char *)list_iter->content, *cmd, ft_strlen(*cmd)) == 0)
		{
			list_iter = list_iter->next;
			if (*(cmd + 1) != NULL)
				cmd++;
		}
		new_node = ft_lstnew(list_iter->content);
		ft_lstadd_back(&new_list, new_node);
		list_iter = list_iter->next;
	}
	ft_lstclear(&envp, &free);
	envp = new_list;
	return (0);
}
