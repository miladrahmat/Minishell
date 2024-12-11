/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 17:13:40 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/12/11 16:50:15 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	append_export(char *str, t_env **envp)
{
	char	*temp;
	t_env	*env_iter;

	env_iter = *envp;
	while (env_iter != NULL)
	{
		if (ft_strncmp(str, env_iter->key, ft_strlen(env_iter->key)) == 0)
			break ;
		else
			env_iter = env_iter->next;
	}
	if (env_iter == NULL)
		return (false);
	while (*str != '=' && *str != '\0')
		str++;
	if (*str == '=')
	{
		str++;
		temp = env_iter->value;
		env_iter->value = ft_strjoin(env_iter->value, str);
		free(temp);
		if (env_iter->value == NULL)
			return (false);
	}
	return (true);
}

static void	print_export(int fd, t_env *envp)
{
	if (ft_strlen(envp->key) == 1 && envp->key[0] == '_')
		return ;
	ft_putstr_fd("declare -x ", fd);
	ft_putstr_fd(envp->key, fd);
	if (envp->flag)
	{
		ft_putstr_fd("=\"", fd);
		ft_putstr_fd(envp->value, fd);
		ft_putendl_fd("\"", fd);
	}
	else
		ft_putchar_fd('\n', fd);
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
	size_t	cmd_nbr;

	cmd_nbr = 1;
	if (get_cmd_amount(cmd) == 1)
		return (env_alpha(fd, ft_envcpy(*envp)));
	while (cmd_nbr < get_cmd_amount(cmd))
	{
		if (ft_strnstr(*(cmd + cmd_nbr), "+=", \
			ft_strlen(*(cmd + cmd_nbr))) != NULL)
		{
			if (append_export(*(cmd + cmd_nbr), envp) == false)
			{
				if (add_new_env(*(cmd + cmd_nbr), envp) < 0)
					return (1);
			}
		}
		else if (export_existing_key(*(cmd + cmd_nbr), envp) == false)
		{
			if (add_new_env(*(cmd + cmd_nbr), envp) < 0)
				return (1);
		}
		cmd_nbr++;
	}
	return (0);
}
