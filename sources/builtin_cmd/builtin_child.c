/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 13:46:43 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/11/20 14:45:21 by lemercie         ###   ########.fr       */
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

int	builtin_exit_child(t_list_and_index child_args, t_env **envp, \
	int last_ret_val)
{
	int		ret_val;
	t_cmd	*cmd_table;

	cmd_table = ft_lstget_nth(child_args.lst, child_args.index)->content;
	ret_val = INT_MIN;
	if (cmd_table->cmd_args[1] != NULL)
	{
		if (validate_str(cmd_table->cmd_args[1], "+-0123456789") == -1)
		{
			print_builtin_error("exit", cmd_table->cmd_args[1], \
				"numeric argument required", false);
			ret_val = 2;
		}
		else
			ret_val = ft_atoi(cmd_table->cmd_args[1]);
	}
	ft_envclear(envp, &free);
	split_free(child_args.env_copy, 0);
	ft_lstclear(&child_args.lst, &destroy_tlist_of_tcmd);
	free(child_args.pids);
	clear_history();
	if (ret_val == INT_MIN)
		exit(last_ret_val);
	exit(ret_val);
}

int	check_builtin_cmd_child(t_list_and_index child_args, t_env **envp, \
	int ret_val)
{
	t_cmd	*cmd_table;
	char	**cmd;
	int		fd;

	cmd_table = ft_lstget_nth(child_args.lst, child_args.index)->content;
	fd = cmd_table->fd.outfile;
	cmd = cmd_table->cmd_args;
	if (ft_strncmp(*cmd, "pwd", 4) == 0)
		return (pwd(fd));
	else if (ft_strncmp("cd", *cmd, 3) == 0)
		return (cd(cmd + 1, envp));
	else if (ft_strncmp(*cmd, "echo", 5) == 0)
		return (echo(cmd + 1, fd));
	else if (ft_strncmp(*cmd, "exit", 5) == 0)
		return (builtin_exit_child(child_args, envp, ret_val));
	else if (ft_strncmp(*cmd, "env", 4) == 0)
		return (env(cmd, fd, envp));
	else if (ft_strncmp(*cmd, "unset", 6) == 0)
		return (unset(cmd + 1, envp));
	else if (ft_strncmp(*cmd, "export", 7) == 0)
		return (export(cmd, fd, envp));
	return (-1);
}
