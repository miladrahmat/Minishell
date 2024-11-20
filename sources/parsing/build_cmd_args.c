/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_cmd_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:27:47 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/20 13:11:12 by lemercie         ###   ########.fr       */
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
// if get_exec_path returns NULL AND path_error == 0: its a malloc() fail
//	otherwise its command not found
//
int	build_cmd_args(t_cmd *cmd, t_env *env)
{	
	if (!cmd->split_token->content || ft_strlen(cmd->split_token->content) <= 0)
	{
		print_builtin_error("", NULL, "command not found", false);
		return (0);
	}
	cmd->cmd_args = malloc(sizeof(char *) * (ft_lstsize(cmd->split_token) + 1));
	if (!cmd->cmd_args)
	{
//		printf("build_cmd_args(): A\n");
		return (1);
	}
	if (!test_builtin_cmd(cmd->split_token->content))
	{
		cmd->path_error = 0;
		cmd->cmd_args[0] = get_exec_path(
				cmd->split_token->content, env, &cmd->path_error);
		if (!cmd->cmd_args[0] && cmd->path_error == 0)
		{
//		if (!cmd->cmd_args[0])a
//			printf("build_cmd_args(): B\n");
			return (1);
		}
	}
	else
	{
		cmd->cmd_args[0] = ft_strdup(cmd->split_token->content);
		if (!cmd->cmd_args[0])
		{
//			printf("build_cmd_args(): C\n");
			return (1);
		}
	}
	return (add_arguments(cmd));
}
