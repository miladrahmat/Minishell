/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 12:20:32 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/10/21 15:59:52 by lemercie         ###   ########.fr       */
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
		perror(strerror(errno));
		free(res);
		return (0);
	}
	ft_putendl_fd(res, fd);
	free(res);
	return (0);
}

int	cd(char **cmd, t_env **envp)
{
	t_env	*list_iter;
	char	*res;
	char	*old_pwd;

	chdir(*cmd);
	res = malloc(PATH_MAX * sizeof(char));
	if (res == NULL)
		return (0);
	getcwd(res, PATH_MAX);
	if (errno != 0)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(*cmd, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
		errno = 0;
		return (0);
	}
	old_pwd = NULL;
	list_iter = *envp;
	while (list_iter != NULL)
	{
		if (ft_strcmp(list_iter->key, "PWD") == 0 && old_pwd == NULL)
		{
			old_pwd = list_iter->value;
			list_iter->value = res;
			list_iter = *envp;
		}
		if (ft_strcmp(list_iter->key, "OLDPWD") == 0 && old_pwd != NULL)
		{
			free(list_iter->value);
			list_iter->value = old_pwd;
		}
		list_iter = list_iter->next;
	}
	return (0);
}

int	builtin_exit(char **cmd, t_env **envp)
{
	int	ret_val;

	ret_val = 0;
	if (cmd[1] != NULL)
		ret_val = ft_atoi(cmd[1]);
	ft_putendl_fd(*cmd, 2);
	split_free(cmd);
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
