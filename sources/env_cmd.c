/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 11:02:40 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/10/03 19:08:25 by mrahmat-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	get_cmd_amount(char **cmd)
{
	size_t	amount;

	amount = 0;
	while (*(cmd + amount) != NULL)
		amount++;
	return (amount);
}

static bool	check_arg(char **cmd, t_list *node)
{
	size_t	cmd_nbr;

	cmd_nbr = 0;
	while (cmd_nbr < get_cmd_amount(cmd) && node != NULL)
	{
		if (ft_strncmp((char *)node->content, \
			*(cmd + cmd_nbr), ft_strlen(*(cmd + cmd_nbr))) == 0)
			return (false);
		cmd_nbr++;
	}
	return (true);
}

int	env(char **cmd, int fd, t_list **envp)
{
	t_list	*list_iter;

	if (cmd[1] != NULL)
	{
		print_error("env", "Too many arguments");
		return (0);
	}
	list_iter = *envp;
	while (list_iter != NULL)
	{
		ft_putendl_fd((char *)list_iter->content, fd);
		list_iter = list_iter->next;
	}
	return (0);
}

int	unset(char **cmd, t_list **envp)
{
	t_list	*list_iter;

	list_iter = *envp;
	while (list_iter != NULL)
	{
		if (!check_arg(cmd, list_iter))
			connect_list(envp, &list_iter);
		else
			list_iter = list_iter->next;
	}
	return (0);
}

int	export(char **cmd, int fd, t_list **envp)
{
	char	*copy;
	t_list	*list_iter;
	size_t	cmd_nbr;
	size_t	cmd_index;

	cmd_nbr = get_cmd_amount(cmd);
	cmd_index = 1;
	list_iter = *envp;
	if (cmd_nbr == 1)
		return (env(cmd, fd, envp));
	while (cmd_index < cmd_nbr)
	{
		while (check_arg(&cmd[cmd_index], list_iter))
		{
			list_iter = list_iter->next;
			if (list_iter == NULL)
				break ;
		}
		copy = ft_strdup(cmd[cmd_index]);
		if (copy == NULL)
			return (0);
		else
			ft_lstadd_back(envp, ft_lstnew(copy));
		cmd_index++;
	}
	return (0);
}
