/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 13:46:43 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/11/18 17:33:02 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_child_signal(int ret_val)
{
	if (ret_val == 130)
	{
		ft_putchar('\n');
		return ;
	}
	if (ret_val == 131)
	{
		ft_putendl_fd("Quit (core dumped)", 2);
		return ;
	}
}

int	builtin_exit_child(t_cmd *cmd_table, t_env **envp, int last_ret_val)
{
	int		ret_val;
	char	**cmd;

	cmd = cmd_table->cmd_args;
	ret_val = INT_MIN;
	if (cmd[1] != NULL)
	{
		if (validate_str(cmd[1], "+-0123456789") == -1)
		{
			print_builtin_error("exit", cmd[1], "numeric argument required", \
				false);
			ret_val = 2;
		}
		else
			ret_val = ft_atoi(cmd[1]);
	}
	ft_envclear(envp, &free);
	// not sure what we should be trying to destroy here
	destroy_tlist_of_tcmd(cmd_table);
	clear_history();
	if (ret_val == INT_MIN)
		exit(last_ret_val);
	exit(ret_val);
}

int	check_builtin_cmd_child(t_cmd *cmd_table, t_env **envp, int ret_val)
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
		return (builtin_exit_child(cmd_table, envp, ret_val));
	else if (ft_strncmp(*cmd, "env", 4) == 0)
		return (env(cmd, fd, envp));
	else if (ft_strncmp(*cmd, "unset", 6) == 0)
		return (unset(cmd + 1, envp));
	else if (ft_strncmp(*cmd, "export", 7) == 0)
		return (export(cmd, fd, envp));
	return (-1);
}
