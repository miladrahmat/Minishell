/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 15:59:59 by lemercie          #+#    #+#             */
/*   Updated: 2024/10/11 15:02:13 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./minishell.h"

bool	is_abs_or_pwd_path(char *cmd)
{
	if (cmd && ft_strlen(cmd) > 1)
	{
		if (ft_strchr(cmd, '/'))
			return (true);
	}
	return (false);
}

int	check_exec_access(char *cmd)
{
	if (access(cmd, F_OK) == 0)
	{
		if (access(cmd, X_OK) == 0)
			return (0);
		return (126);
	}
	return (127);
}

char	**get_paths(t_env *env)
{
	return (ft_split(ft_env_get_value_by_key("PATH", env), ':'));
	/*
	char	*line;

	t_list	*env_iter;

	env_iter = env;
	while (env_iter)
	{
		line = (char *) env_iter->content;
		if (ft_strncmp(line, "PATH=", 5) == 0)
			return (ft_split(line, ':'));
		env_iter = env_iter->next;
	}
	return (NULL);
	*/
}
/*
char	**get_paths(char **envp)
{
	while (*envp)
	{
		if (ft_strncmp(*envp, "PATH=", 5) == 0)
			return (ft_split(*envp, ':'));
		envp++;
	}
	return (NULL);
}
*/
