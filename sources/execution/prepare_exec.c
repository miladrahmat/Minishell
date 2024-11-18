/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:21:33 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/11/18 17:17:14 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	wait_pids(pid_t **pid, int index)
{
	int	iter;
	int	ret_val;

	if (index < 0)
	{
		free(*pid);
		return (0);
	}
	iter = -1;
	ret_val = 0;
	while (++iter < index)
	{
		if ((*pid)[iter] != 0)
			waitpid((*pid)[iter], NULL, 0);
	}
	if ((*pid)[iter] == 0)
		return (1);
	waitpid((*pid)[iter], &ret_val, 0);
	free(*pid);
	handle_sigint(&handle_signals);
	if (WIFSIGNALED(ret_val))
		return (128 + WTERMSIG(ret_val));
	if (WIFEXITED(ret_val))
		return (WEXITSTATUS(ret_val));
	return (-1);
}

static void	execute_cmd(t_list_and_index child_args, char **env_copy, \
	t_env **env, int last_ret_val)
{
	int		ret_val;
	t_cmd	*cmd;

	cmd = ft_lstget_nth(child_args.lst, child_args.i)->content;
	signal_handling_child();
	ret_val = 127;
	if (cmd->fd.infile == -1 || cmd->fd.outfile == -1)
		exit(1);
	if (cmd->cmd_args[0] != NULL)
		ret_val = check_builtin_cmd_child(cmd, env, last_ret_val);
	if (ret_val < 0 && (cmd->fd.infile >= 0 && cmd->fd.outfile > 0))
	{
		ft_envclear(env, &free);
		dup2(cmd->fd.infile, STDIN_FILENO);
		dup2(cmd->fd.outfile, STDOUT_FILENO);
		close_cmd_fd(cmd);
		if (cmd->cmd_args[0] != NULL)
			execve(cmd->cmd_args[0], cmd->cmd_args, env_copy);
	}
	close_cmd_fd(cmd);
	split_free(env_copy, 0);
	ft_envclear(env, &free);
	ft_lstclear(&child_args.lst, &destroy_tlist_of_tcmd);
	free(child_args.pids);
	exit(ret_val);
}

static pid_t	prepare_child(t_list_and_index child_args, t_env **env, \
	char **env_copy, int ret_val)
{
	pid_t	pid;
	t_list	*cmd;

	cmd = ft_lstget_nth(child_args.lst, child_args.i);
	if (cmd->next != NULL && (check_pipe_fd(((t_cmd **)&cmd->content), \
			((t_cmd **)&cmd->next->content)) < 0))
		return (-1);
	if (open_files(&cmd) < 0)
	{
		close_cmd_fd(cmd->content);
		return (0);
	}
	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
	{
		if (cmd->next != NULL)
			close(((t_cmd *)cmd->next->content)->fd.infile);
		execute_cmd(child_args, env_copy, env, ret_val);
	}
	ignore_sigint();
	close_cmd_fd(((t_cmd *)cmd->content));
	return (pid);
}

static int	prepare_parent(t_list *cmd_table, t_env **env, \
	char **env_copy, int ret_val)
{
	pid_t				*child;
	t_list				*cmd_iter;
	t_list_and_index	c_args;

	child = malloc(ft_lstsize(cmd_table) * (sizeof(pid_t)));
	if (child == NULL)
		return (1);
	c_args.i = -1;
	c_args.lst = cmd_table;
	c_args.pids = child;
	cmd_iter = cmd_table;
	while (++c_args.i < ft_lstsize(cmd_table) && cmd_iter != NULL)
	{
		if (((t_cmd *)cmd_iter->content)->cmd_args != NULL)
		{
			child[c_args.i] = prepare_child(c_args, env, env_copy, ret_val);
			if (child[c_args.i] < 0)
			{
				wait_pids(&child, c_args.i - 1);
				return (1);
			}
		}
		cmd_iter = cmd_iter->next;
	}
	return (wait_pids(&child, c_args.i - 1));
}

int	prepare_exec(t_list *cmd_table, t_env **env, int last_ret_val)
{
	char	**env_copy;
	size_t	env_i;
	t_env	*env_iter;
	int		ret_val;

	ret_val = execute_one_builtin(cmd_table, env, last_ret_val);
	if (ret_val >= 0)
		return (ret_val);
	env_copy = malloc((ft_envsize(*env) + 1) * sizeof(char *));
	if (env_copy == NULL)
		return (1);
	env_iter = *env;
	env_i = 0;
	while (env_iter != NULL)
	{
		env_copy[env_i] = copy_env_node(env_iter);
		if (env_copy[env_i] == NULL)
			return (split_free(env_copy, 1));
		env_iter = env_iter->next;
		env_i++;
	}
	env_copy[env_i] = NULL;
	ret_val = prepare_parent(cmd_table, env, env_copy, last_ret_val);
	return (split_free(env_copy, ret_val));
}
