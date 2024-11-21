/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 14:38:19 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/21 19:02:07 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./minishell.h"

static char	*create_path(char *dir, char *cmd)
{
	int		exec_path_len;
	char	*exec_path;

	exec_path_len = ft_strlen(dir) + ft_strlen(cmd) + 2;
	exec_path = malloc(sizeof(char) * exec_path_len);
	if (!exec_path)
		return (NULL);
	ft_strlcpy(exec_path, dir, exec_path_len);
	ft_strlcat(exec_path, "/", exec_path_len);
	ft_strlcat(exec_path, cmd, exec_path_len);
	return (exec_path);
}

static int	find_in_paths(char **paths, char **exec_args, int *path_error)
{
	char	*exec_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		exec_path = create_path(paths[i], *exec_args);
		if (!exec_path)
			return (2);
		*path_error = check_exec_access(exec_path);
		if (*path_error == 126)
		{
			free(exec_path);
			return (1);
		}
		if (*path_error == 0)
		{
			free(*exec_args);
			*exec_args = exec_path;
			return (0);
		}
		if (*path_error == 126)
			return (1);
		free(exec_path);
		i++;
	}
	*path_error = 127;
	return (1);
}

static char	*search_paths(char *exec_args, t_env *env, int *path_error)
{
	char	**paths;
	int		err;

	paths = get_paths(env, &err);
	if (err)
		return (NULL);
	if (!paths)
	{
		*path_error = check_exec_access_print_err(exec_args);
		if (*path_error == 0)
			return (exec_args);
		free(exec_args);
		return (NULL);
	}
	*path_error = 0;
	err = find_in_paths(paths, &exec_args, path_error);
	split_free(paths, 0);
	if (err > 0)
	{
		if (err == 1)
			print_builtin_error(exec_args, NULL, "command not found", false);
		free(exec_args);
		return (NULL);
	}
	return (exec_args);
}

// 4 possibilities
// 		absolute path to executable
// 		executable found in PATH
// 		executable in current dir with ./
// 		executable under current dir starting with a directory name without ./
// if the cmd is an empty string ==> return 126
// if the cmd is a space ==> return 127
// if the cmd is a real file but not executable ==> return 126
// if the cmd is not found ==> return 127
//
// if cmd has a /, then check first without using paths
// if PATH is unset, do the same
// then check in PATH
static char	*get_exec_path_more(char *command, t_env *env, int *path_error)
{	
	char	*exec_args;

	exec_args = ft_strdup(command);
	if (!exec_args)
	{
		print_builtin_error(NULL, NULL, "command not found", false);
		*path_error = 127;
		return (NULL);
	}
	if (is_abs_or_pwd_path(exec_args))
	{
		*path_error = check_exec_access_print_err(exec_args);
		if (*path_error == 0)
			return (exec_args);
		free(exec_args);
		return (NULL);
	}
	return (search_paths(exec_args, env, path_error));
}

// If this returns NULL and path_error = 0, that is a malloc fail
char	*get_exec_path(char *command, t_env *env, int *path_error)
{
	char	*temp;
	char	*ret;

	if (!command || !command[0])
	{
		print_builtin_error(NULL, NULL, "command not found", false);
		*path_error = 126;
		return (NULL);
	}
	temp = get_exec_path_more(command, env, path_error);
	if (temp)
	{
		ret = ft_strdup(temp);
		free(temp);
		return (ret);
	}
	return (NULL);
}
