/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_table.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 09:48:22 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/07 15:06:05 by mrahmat-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Must be passed to ft_lstmap(). 
// For each node in t_list tokens, creates a node in t_list of t_cmd. 
// Init each t_cmd of the list (but not cmd->cmd_args yet). 
void	*init_t_cmd(void *content)
{
	t_cmd	*cmd;
	
	cmd = malloc(sizeof(t_cmd));
	cmd->split_token = split_token(content);
	cmd->cmd_args = NULL;
	cmd->files = NULL;
	cmd->path_error = 0;
	cmd->fd.infile = 0;
	cmd->fd.outfile = 1;
	return (cmd);
}

// return 1 on malloc fails
int	transform_tokens(t_list **head, t_env *env, int *last_ret_val)
{
	t_list	*split_tokens_iter;
	char	*expanded_token;
	char	*unquoted_token;

	split_tokens_iter = *head;
	while (split_tokens_iter)
	{
		expanded_token = expand_vars(
			split_tokens_iter->content, env, last_ret_val);
		if (!expanded_token)
			return (1);
		if (ft_strlen(expanded_token) == 0)
		{
			ft_lstdel_and_connect(head, &split_tokens_iter);
			if (split_tokens_iter && split_tokens_iter->next)
				split_tokens_iter = split_tokens_iter->next;
			continue ;
		}
		unquoted_token = strip_quotes(&expanded_token, last_ret_val);
		if (ft_strlen(expanded_token) == 0)
			ft_lstdel_and_connect(head, &split_tokens_iter);
		else
		{
			if (split_tokens_iter->content)
				free(split_tokens_iter->content);
			split_tokens_iter->content = unquoted_token;
		}
		if (split_tokens_iter && split_tokens_iter->next)
			split_tokens_iter = split_tokens_iter->next;
		else
			break ;
	}
	return (0);
}

// Can return NULL in case of a failed malloc() in functions called from here
//
// When an incorrect variable name is given, expand_vars() will return 
// an empty string. 
//
// After parsing redirs AND expanding variables, we can assume that 
// the first token is the cmd. 
t_list	*init_cmd_table(char *line, t_env *env, int last_ret_val)
{
	t_list	*pipe_tokens;
	t_list	*cmd_table;
	t_list	*cmd_table_iter;
	t_cmd	*cmd;

	pipe_tokens = split_on_pipes(line);
	cmd_table = ft_lstmap(pipe_tokens, &init_t_cmd, &free);
	cmd_table_iter = cmd_table;
	while (cmd_table_iter)
	{
		cmd = (t_cmd *) cmd_table_iter->content;
		if (!cmd->split_token)
		{
			cmd_table_iter = cmd_table_iter->next;
			continue ;
		}
		transform_tokens(&cmd->split_token, env, &last_ret_val);
		cmd_table_iter = cmd_table_iter->next;
	}
	ft_lstiter(cmd_table, parse_redir_loop);
	cmd_table_iter = cmd_table;
	while (cmd_table_iter)
	{
		cmd = (t_cmd *) cmd_table_iter->content;
		if (!cmd->split_token)
		{
			cmd_table_iter = cmd_table_iter->next;
			continue ;
		}
		build_cmd_args(cmd, env);
		cmd_table_iter = cmd_table_iter->next;
	}
	return (cmd_table);
}
