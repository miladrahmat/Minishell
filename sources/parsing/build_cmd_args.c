/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_cmd_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:27:47 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/12 14:07:27 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	add_arguments(t_cmd *cmd)
{
	int		i;
	t_list	*split_tokens_iter;

	i = 1;
	split_tokens_iter = cmd->split_token;
	split_tokens_iter = split_tokens_iter->next;
	while (split_tokens_iter)
	{
		cmd->cmd_args[i] = ft_strdup(split_tokens_iter->content);
		if (!cmd->cmd_args[i])
			return (1);
		split_tokens_iter = split_tokens_iter->next;
		i++;
	}
	cmd->cmd_args[i] = NULL;
	return (0);
}

// checks first token for a cmd, then just appends the rest of the tokens
int	build_cmd_args(t_cmd *cmd, t_env *env)
{	
	if (!cmd->split_token->content || ft_strlen(cmd->split_token->content) <= 0)
	{
		ft_putstr_fd("Error: command \"\" not found\n", 2);
		return (1);
	}
	cmd->cmd_args = malloc(sizeof(char *) * (ft_lstsize(cmd->split_token) + 1));
	if (!cmd->cmd_args)
		return (1);
	if (!test_builtin_cmd(cmd->split_token->content))
	{
		cmd->path_error = 0;
		cmd->cmd_args[0] = get_exec_path(
				cmd->split_token->content, env, &cmd->path_error);
		if (!cmd->cmd_args[0] && cmd->path_error == 0)
			return (1);
	}
	else
	{
		cmd->cmd_args[0] = ft_strdup(cmd->split_token->content);
		if (!cmd->cmd_args[0])
			return (1);
	}
	return (add_arguments(cmd));
}
