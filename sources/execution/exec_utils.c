/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 17:23:18 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/12/04 13:55:11 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_all_fds(t_list *cmd_table)
{
	t_list	*cmd_iter;

	cmd_iter = cmd_table;
	while (cmd_iter != NULL)
	{
		close_cmd_fd(((t_cmd *)cmd_iter->content));
		cmd_iter = cmd_iter->next;
	}
}

int	pid_less_than_zero(pid_t **pid, size_t index)
{
	if ((*pid)[index] == 0)
	{
		free(*pid);
		return (0);
	}
	else
	{
		free(*pid);
		return (1);
	}
}

int	check_child_exit_code(int status)
{
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (-1);
}

int	execute_one_builtin(t_list *cmd_table, t_env **env, int ret_val)
{
	int	check;

	check = -1;
	if (ft_lstsize(cmd_table) != 1)
		return (-1);
	else if (open_files(&cmd_table) < 0)
	{
		close_cmd_fd((t_cmd *)cmd_table->content);
		return (1);
	}
	else if (((t_cmd *)cmd_table->content)->cmd_args != NULL \
		&& ((t_cmd *)cmd_table->content)->cmd_args[0] != NULL)
		check = check_builtin_cmd(&cmd_table, env, ret_val);
	return (check);
}

char	*copy_env_node(t_env *env)
{
	char	*node_cpy;
	char	*temp;

	node_cpy = ft_strdup(env->key);
	if (node_cpy == NULL)
		return (NULL);
	temp = node_cpy;
	node_cpy = ft_strjoin(node_cpy, "=");
	free(temp);
	if (node_cpy == NULL)
		return (NULL);
	temp = node_cpy;
	node_cpy = ft_strjoin(node_cpy, env->value);
	free(temp);
	if (node_cpy == NULL)
		return (NULL);
	return (node_cpy);
}
