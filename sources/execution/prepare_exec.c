/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:21:33 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/10/24 17:44:20 by mrahmat-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	wait_pids(pid_t **pid, int index)
{
	int	iter;
	int	ret_val;

	if (index < 0)
	{
		free(*pid);
		return (0);
	}
	iter = 0;
	ret_val = 0;
	while (iter < index)
	{
		waitpid((*pid)[iter], NULL, 0);
		iter++;
	}
	waitpid((*pid)[iter], &ret_val, 0);
	free(*pid);
	if (WIFEXITED(ret_val))
		return (WEXITSTATUS(ret_val));
	return (ret_val);
}

void	execute_cmd(t_cmd *cmd, char **env_copy, t_env **env)
{
	int	ret_val;

	ret_val = 1;
	if (check_builtin_cmd(cmd->cmd_args, cmd->fd->outfile, env) < 0)
	{
		if (cmd->fd->infile >= 0 && cmd->fd->outfile > 0)
		{
			dup2(cmd->fd->infile, STDIN_FILENO);
			if (cmd->fd->infile != -1 && cmd->fd->infile > 0)
				close(cmd->fd->infile);
			dup2(cmd->fd->outfile, STDOUT_FILENO);
			if (cmd->fd->outfile != -1 && cmd->fd->outfile > 1)
				close(cmd->fd->outfile);
			if (cmd->cmd_args != NULL)
				execve(cmd->cmd_args[0], cmd->cmd_args, env_copy);
			ret_val = 127;
		}
	}
	if (cmd->fd->infile != -1 && cmd->fd->infile > 0)
		close(cmd->fd->infile);
	if (cmd->fd->outfile != -1 && cmd->fd->outfile > 1)
		close(cmd->fd->outfile);
	split_free(env_copy);
	ft_envclear(env, &free);
	//need to free t_cmd
	exit(ret_val);
}

int	prepare_children(t_list *cmd_table, t_env **env, char **env_copy)
{
	pid_t	*child;
	t_list	*cmd_iter;
	int		child_i;
	int		cmd_count;

	if (cmd_table == NULL)
		return (127);
	cmd_count = ft_lstsize(cmd_table);
	child_i = 0;
	child = malloc(cmd_count * (sizeof(pid_t)));
	if (child == NULL)
		return (1);
	cmd_iter = cmd_table;
	while (child_i < cmd_count)
	{
		if (cmd_iter->next != NULL)
			if (check_pipe_fd(((t_cmd **)&cmd_iter->content), \
				((t_cmd **)&cmd_iter->next->content)) < 0)
				break ;
		child[child_i] = fork();
		if (child[child_i] < 0)
		{
			wait_pids(&child, child_i - 1);
			return (1);
		}
		if (child[child_i] == 0)
			execute_cmd(((t_cmd *)cmd_iter->content), env_copy, env);
		close_cmd_fd(((t_cmd *)cmd_iter->content));
		cmd_iter = cmd_iter->next;
		child_i++;
	}
	return (wait_pids(&child, child_i - 1));
}

int	execute_one_builtin(t_list *cmd_table, t_env **env)
{
	if (ft_lstsize(cmd_table) == 1)
		return (check_builtin_cmd(((t_cmd *)cmd_table->content)->cmd_args, \
			((t_files *)((t_cmd *)cmd_table->content)->fd)->outfile, env));
	return (-1);
}

int	prepare_exec(t_list *cmd_table, t_env **env)
{
	char	**env_copy;
	size_t	env_i;
	t_env	*env_iter;
	int		ret_val;

	if (execute_one_builtin(cmd_table, env) >= 0)
		return (0);
	env_copy = malloc((ft_envsize(*env) + 1) * sizeof(char *));
	if (env_copy == NULL)
		return (1);
	env_iter = *env;
	env_i = 0;
	while (env_iter != NULL)
	{
		env_copy[env_i] = copy_env_node(env_iter);
		if (env_copy[env_i++] == NULL)
		{
			split_free(env_copy);
			return (1);
		}
		env_iter = env_iter->next;
	}
	env_copy[env_i] = NULL;
	ret_val = prepare_children(cmd_table, env, env_copy);
	split_free(env_copy);
	return (ret_val);
}
