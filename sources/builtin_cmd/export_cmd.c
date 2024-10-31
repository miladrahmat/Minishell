/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 17:13:40 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/10/30 18:46:08 by mrahmat-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static void	print_export(int fd, t_env *envp)
{
	if (envp->key[0] == '_')
		return ;
	ft_putstr_fd("declare -x ", fd);
	ft_putstr_fd(envp->key, fd);
	ft_putstr_fd("=\"", fd);
	ft_putstr_fd(envp->value, fd);
	ft_putendl_fd("\"", fd);
}

static int	env_alpha(int fd, t_env *envp)
{
	t_env	*list_iter;
	t_env	*smallest_key;

	if (envp == NULL)
		return (0);
	list_iter = envp;
	smallest_key = list_iter;
	list_iter = list_iter->next;
	while (list_iter != NULL)
	{
		if (ft_strcmp(smallest_key->key, list_iter->key) > 0)
		{
			smallest_key = list_iter;
			list_iter = list_iter->next;
		}
		else
			list_iter = list_iter->next;
	}
	print_export(fd, smallest_key);
	connect_list(&envp, &smallest_key);
	env_alpha(fd, envp);
	return (0);
}

int	export(char **cmd, int fd, t_env **envp)
{
	t_env	*new;
	size_t	cmd_nbr;

	cmd_nbr = 1;
	if (get_cmd_amount(cmd) == 1)
		return (env_alpha(fd, ft_envcpy(*envp)));
	while (cmd_nbr < get_cmd_amount(cmd))
	{
		if (export_existing_key(*(cmd + cmd_nbr), envp) == false)
		{
			new = set_key_value(*(cmd + cmd_nbr));
			if (new != NULL)
				ft_envadd_back(envp, new);
			else
				return (1);
		}
		cmd_nbr++;
	}
	return (0);
}
