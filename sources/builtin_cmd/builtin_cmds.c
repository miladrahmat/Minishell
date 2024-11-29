/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 12:20:32 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/11/29 14:31:20 by mrahmat-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pwd(int fd)
{
	char	*res;

	res = malloc(PATH_MAX * sizeof(char));
	if (res == NULL)
		return (0);
	if (getcwd(res, PATH_MAX) == NULL)
	{
		if (errno != 0)
		{
			free(res);
			return (print_builtin_error("pwd", NULL, NULL, false));
		}
	}
	ft_putendl_fd(res, fd);
	free(res);
	return (0);
}

int	cd(char **cmd, t_env **envp)
{
	if (cmd[0] != NULL)
	{
		if (cmd[1] != NULL)
			return (print_builtin_error("cd", NULL, \
				"too many arguments", false));
		if (chdir(*cmd) == -1)
		{
			if (errno != 0)
				return (print_builtin_error("cd", *cmd, NULL, false));
		}
		update_pwd(envp);
	}
	return (0);
}

int	builtin_exit(t_list **cmd_table, t_env **envp, int last_ret_val)
{
	long long		ret_val;
	char			**cmd;
	int				check;

	cmd = ((t_cmd *)(*cmd_table)->content)->cmd_args;
	if (cmd[1] != NULL)
		check = 1;
	else
		check = 0;
	ft_putendl_fd(*cmd, 2);
	ret_val = exit_error_check(cmd);
	ft_envclear(envp, &free);
	ft_lstclear(cmd_table, &destroy_tlist_of_tcmd);
	clear_history();
	if (check == 0)
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

int	check_builtin_cmd(t_list **cmd_table, t_env **envp, int last_ret_val)
{
	char	**cmd;
	int		fd;
	int		ret_val;

	ret_val = -1;
	cmd = ((t_cmd *)(*cmd_table)->content)->cmd_args;
	fd = ((t_cmd *)(*cmd_table)->content)->fd.outfile;
	if (ft_strncmp(*cmd, "pwd", 4) == 0)
		ret_val = pwd(fd);
	else if (ft_strncmp("cd", *cmd, 3) == 0)
		ret_val = cd(cmd + 1, envp);
	else if (ft_strncmp(*cmd, "echo", 5) == 0)
		ret_val = echo(cmd + 1, fd);
	else if (ft_strncmp(*cmd, "exit", 5) == 0)
		ret_val = builtin_exit(cmd_table, envp, last_ret_val);
	else if (ft_strncmp(*cmd, "env", 4) == 0)
		ret_val = env(cmd, fd, envp);
	else if (ft_strncmp(*cmd, "unset", 6) == 0)
		ret_val = unset(cmd + 1, envp);
	else if (ft_strncmp(*cmd, "export", 7) == 0)
		ret_val = export(cmd, fd, envp);
	if (ret_val >= 0)
		close_cmd_fd((*cmd_table)->content);
	return (ret_val);
}
