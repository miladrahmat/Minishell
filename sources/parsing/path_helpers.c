/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 15:59:59 by lemercie          #+#    #+#             */
/*   Updated: 2024/12/02 11:54:25 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./minishell.h"

static bool	is_directory(char *path)
{
	struct stat	sb;

	if (stat(path, &sb) == -1)
	{
		errno = 0;
		return (false);
	}
	return (S_ISDIR(sb.st_mode));
}

bool	is_abs_or_pwd_path(char *cmd)
{
	if (cmd && ft_strcmp(cmd, ".") == 0)
		return (true);
	if (cmd && ft_strlen(cmd) > 1)
	{
		if (ft_strchr(cmd, '/'))
			return (true);
	}
	return (false);
}

int	check_exec_access(char *cmd)
{
	if (is_directory(cmd))
	{
		return (126);
	}
	if (access(cmd, F_OK) == 0)
	{
		if (access(cmd, X_OK) == 0)
			return (0);
		return (126);
	}
	return (127);
}

int	check_exec_access_print_err(char *cmd)
{
	if (is_directory(cmd))
	{
		print_builtin_error(cmd, NULL, "Is a directory", false);
		return (126);
	}
	if (access(cmd, F_OK) == 0)
	{
		if (access(cmd, X_OK) == 0)
			return (0);
		print_builtin_error(cmd, NULL, "Permission denied", false);
		return (126);
	}
	print_builtin_error(cmd, NULL, "No such file or directory", false);
	return (127);
}

char	**get_paths(t_env *env, int *err)
{
	char	*path;
	char	**split_path;

	*err = 0;
	path = ft_env_get_value_by_key("PATH", env);
	if (!path)
		return (NULL);
	split_path = ft_split(path, ':');
	if (!split_path)
	{
		*err = 1;
		return (NULL);
	}
	return (split_path);
}
