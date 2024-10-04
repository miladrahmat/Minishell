/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 11:02:40 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/10/04 19:06:09 by mrahmat-         ###   ########.fr       */
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
			if (!check_key(*(cmd + cmd_iter), list_iter))
				connect_list(envp, &list_iter);
			cmd_iter++;
		}
		if (list_iter != NULL)
			list_iter = list_iter->next;
	}
	return (0);
}

static bool	export_existing_key(char *cmd, t_env **envp)
{
	t_env	*list_iter;
	char	*key;
	char	*value;
	size_t	cut;

	list_iter = *envp;
	cut = ft_strlen_eq(cmd);
	while (list_iter != NULL)
	{
		if (!check_key(cmd, list_iter))
		{
			key = ft_substr(cmd, 0, cut);
			if (key == NULL)
				return (0);
			value = ft_substr(cmd, cut + 1, ft_strlen(cmd) - cut);
			if (value == NULL)
				return (0);
			ft_env_free_add(list_iter, key, value);
			return (true);
		}
		if (list_iter != NULL)
			list_iter = list_iter->next;
	}
	return (false);
}

static int	print_export(int fd, t_env **envp)
{
	t_env	*list_iter;

	list_iter = *envp;
	while (list_iter != NULL)
	{
		ft_putstr_fd("declare -x ", fd);
		ft_putstr_fd(list_iter->key, fd);
		ft_putstr_fd("=\"", fd);
		ft_putstr_fd(list_iter->value, fd);
		ft_putendl_fd("\"", fd);
		list_iter = list_iter->next;
	}
	return (0);
}

int	export(char **cmd, int fd, t_env **envp)
{
	t_env	*new;
	size_t	cmd_nbr;

	cmd_nbr = 1;
	if (get_cmd_amount(cmd) == 1)
		return (print_export(fd, envp));
	while (cmd_nbr < get_cmd_amount(cmd))
	{
		if (export_existing_key(*(cmd + cmd_nbr), envp) == false)
		{
			new = get_key_value(*(cmd + cmd_nbr));
			if (new == NULL)
				return (0);
			ft_envadd_back(envp, new);
		}
		cmd_nbr++;
	}
	return (0);
}
