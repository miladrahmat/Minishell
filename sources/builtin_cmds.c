/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 12:20:32 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/10/03 16:41:42 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pwd(char **cmd, int fd)
{
	char	*res;

	if (cmd[1] != NULL)
	{
		ft_putendl_fd("pwd: too many arguments", 2);
		return (0);
	}
	res = malloc(1000 * sizeof(char));
	if (res == NULL)
	{
		perror(strerror(errno));
		errno = 0;
		return (0);
	}
	getcwd(res, 1000);
	if (errno != 0)
	{
		print_error("pwd", strerror(errno));
		errno = 0;
		return (0);
	}
	ft_putendl_fd(res, fd);
	free(res);
	return (0);
}

int	cd(char **cmd)
{
	chdir(*cmd);
	if (errno != 0)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(*cmd, 2);
		errno = 0;
		return (0);
	}
	return (0);
}

int	builtin_exit(char **cmd)
{
	int	ret_val;

	ret_val = 0;
	if (cmd[1] != NULL)
		ret_val = ft_atoi(cmd[1]);
	ft_putendl_fd(*cmd, 2);
	split_free(cmd);
	exit(ret_val);
}

int	check_builtin_cmd(char **cmd, int fd, t_list **envp)
{
	if (ft_strncmp(*cmd, "pwd", 4) == 0)
		return (pwd(cmd, fd));
	else if (ft_strncmp("cd", *cmd, 3) == 0)
		return (cd(cmd + 1));
	else if (ft_strncmp(*cmd, "echo", 5) == 0)
		return (echo(cmd + 1, fd));
	else if (ft_strncmp(*cmd, "exit", 5) == 0)
		return (builtin_exit(cmd));
	else if (ft_strncmp(*cmd, "env", 4) == 0)
		return (env(cmd, fd, envp));
	else if (ft_strncmp(*cmd, "unset", 6) == 0)
		return (unset(cmd + 1, envp));
	return (-1);
}
