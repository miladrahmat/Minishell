/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 12:20:32 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/10/22 11:37:48 by mrahmat-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pwd(int fd)
{
	char	*res;

	res = malloc(PATH_MAX * sizeof(char));
	if (res == NULL)
		return (0);
	getcwd(res, PATH_MAX);
	if (errno != 0)
	{
		free(res);
		return (print_builtin_error("pwd", NULL, NULL, false));
	}
	ft_putendl_fd(res, fd);
	free(res);
	return (0);
}

int	cd(char **cmd, t_env **envp)
{
	chdir(*cmd);
	if (errno != 0)
		return (print_builtin_error("cd", *cmd, NULL, false));
	update_pwd(envp);
	return (0);
}

int	builtin_exit(char **cmd, t_env **envp)
{
	int	ret_val;

	ft_putendl_fd(*cmd, 2);
	ret_val = 0;
	if (cmd[1] != NULL)
	{
		if (validate_str(cmd[1], "0123456789") == -1)
		{
			print_builtin_error("exit", cmd[1], "numeric argument required", \
				false);
			ret_val = 2;
		}
		else
			ret_val = ft_atoi(cmd[1]);
	}
	ft_envclear(envp, &free);
	exit(ret_val);
}

// returns false also when the ft_split in the caller fails
bool	test_builtin_cmd(char *cmd)
{
	bool	ret;

	if (!cmd)
		return (false);
	if (ft_strncmp(cmd, "pwd", 4) == 0)
		ret = true;
	else if (ft_strncmp("cd", cmd, 3) == 0)
		ret = true;
	else if (ft_strncmp(cmd, "echo", 5) == 0)
		ret = true;
	else if (ft_strncmp(cmd, "exit", 5) == 0)
		ret = true;
	else if (ft_strncmp(cmd, "env", 4) == 0)
		ret = true;
	else if (ft_strncmp(cmd, "unset", 6) == 0)
		ret = true;
	else if (ft_strncmp(cmd, "export", 7) == 0)
		ret = true;
	else
		ret = false;
	return (ret);
}

int	check_builtin_cmd(char **cmd, int fd, t_env **envp)
{
	if (ft_strncmp(*cmd, "pwd", 4) == 0)
		return (pwd(fd));
	else if (ft_strncmp("cd", *cmd, 3) == 0)
		return (cd(cmd + 1, envp));
	else if (ft_strncmp(*cmd, "echo", 5) == 0)
		return (echo(cmd + 1, fd));
	else if (ft_strncmp(*cmd, "exit", 5) == 0)
		return (builtin_exit(cmd, envp));
	else if (ft_strncmp(*cmd, "env", 4) == 0)
		return (env(cmd, fd, envp));
	else if (ft_strncmp(*cmd, "unset", 6) == 0)
		return (unset(cmd + 1, envp));
	else if (ft_strncmp(*cmd, "export", 7) == 0)
		return (export(cmd, fd, envp));
	return (-1);
}
