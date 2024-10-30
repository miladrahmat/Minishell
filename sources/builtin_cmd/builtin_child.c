/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 13:46:43 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/10/30 14:16:34 by mrahmat-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_exit_child(t_cmd *cmd_table, t_env **envp)
{
	int		ret_val;
	char	**cmd;

	cmd = cmd_table->cmd_args;
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
	destroy_tlist_of_tcmd(cmd_table);
	clear_history();
	exit(ret_val);
}

int	check_builtin_cmd_child(t_cmd *cmd_table, t_env **envp)
{
	char	**cmd;
	int		fd;

	cmd = cmd_table->cmd_args;
	fd = ((t_files)cmd_table->fd).outfile;
	if (ft_strncmp(*cmd, "pwd", 4) == 0)
		return (pwd(fd));
	else if (ft_strncmp("cd", *cmd, 3) == 0)
		return (cd(cmd + 1, envp));
	else if (ft_strncmp(*cmd, "echo", 5) == 0)
		return (echo(cmd + 1, fd));
	else if (ft_strncmp(*cmd, "exit", 5) == 0)
		return (builtin_exit_child(cmd_table, envp));
	else if (ft_strncmp(*cmd, "env", 4) == 0)
		return (env(cmd, fd, envp));
	else if (ft_strncmp(*cmd, "unset", 6) == 0)
		return (unset(cmd + 1, envp));
	else if (ft_strncmp(*cmd, "export", 7) == 0)
		return (export(cmd, fd, envp));
	return (-1);
}
