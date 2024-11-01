/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 12:20:32 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/11/01 15:09:56 by mrahmat-         ###   ########.fr       */
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
	if(cmd[0] != NULL)
	{
		if (cmd[1] != NULL)
			return (print_builtin_error("cd", NULL, "too many arguments", false));
		chdir(*cmd);
		if (errno != 0)
			return (print_builtin_error("cd", *cmd, NULL, false));
		update_pwd(envp);
	}
	return (0);
}

int	builtin_exit(t_list **cmd_table, t_env **envp, int last_ret_val)
{
	int		ret_val;
	char	**cmd;

	cmd = ((t_cmd *)(*cmd_table)->content)->cmd_args;
	ft_putendl_fd(*cmd, 2);
	ret_val = INT_MIN;
	if (cmd[1] != NULL && cmd[2] != NULL)
	{
		print_builtin_error("exit", NULL, "too many arguments", false);
		ret_val = 1;
	}
	else if (cmd[1] != NULL)
	{
		if (validate_str(cmd[1], "-+0123456789") == -1)
		{
			print_builtin_error("exit", cmd[1], "numeric argument required", \
				false);
			ret_val = 2;
		}
		else
			ret_val = ft_atoi(cmd[1]);
	}
	ft_envclear(envp, &free);
	ft_lstclear(cmd_table, &destroy_tlist_of_tcmd);
	clear_history();
	if (ret_val == INT_MIN)
		exit(last_ret_val);
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

int	check_builtin_cmd(t_list **cmd_table, t_env **envp, int ret_val)
{
	char	**cmd;
	int		fd;

	cmd = ((t_cmd *)(*cmd_table)->content)->cmd_args;
	fd = ((t_cmd *)(*cmd_table)->content)->fd.outfile;
	if (ft_strncmp(*cmd, "pwd", 4) == 0)
		return (pwd(fd));
	else if (ft_strncmp("cd", *cmd, 3) == 0)
		return (cd(cmd + 1, envp));
	else if (ft_strncmp(*cmd, "echo", 5) == 0)
		return (echo(cmd + 1, fd));
	else if (ft_strncmp(*cmd, "exit", 5) == 0)
		return (builtin_exit(cmd_table, envp, ret_val));
	else if (ft_strncmp(*cmd, "env", 4) == 0)
		return (env(cmd, fd, envp));
	else if (ft_strncmp(*cmd, "unset", 6) == 0)
		return (unset(cmd + 1, envp));
	else if (ft_strncmp(*cmd, "export", 7) == 0)
		return (export(cmd, fd, envp));
	return (-1);
}
