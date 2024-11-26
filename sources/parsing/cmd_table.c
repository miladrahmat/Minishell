/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_table.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 09:48:22 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/26 15:02:59 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Must be passed to ft_lstmap(). 
// For each node in t_list tokens, creates a node in t_list of t_cmd. 
// Init each t_cmd of the list (but not cmd->cmd_args yet). 
static void	*init_t_cmd(void *content)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->split_token = split_token(content);
	if (!cmd->split_token)
	{
		free(cmd);
		return (NULL);
	}
	cmd->cmd_args = NULL;
	cmd->files = NULL;
	cmd->path_error = 0;
	cmd->fd.infile = 0;
	cmd->fd.outfile = 1;
	return (cmd);
}

static void	*init_cmd_table_destroyer(t_list **cmd_table)
{
	if (*cmd_table)
		ft_lstclear(cmd_table, &destroy_tlist_of_tcmd);
	*cmd_table = NULL;
	return (NULL);
}

// can return NULL in case of malloc fail
static t_list	*create_cmd_table(char *line)
{
	t_list	*pipe_tokens;
	t_list	*cmd_table;

	pipe_tokens = split_on_pipes(line);
	if (!pipe_tokens)
		return (NULL);
	cmd_table = ft_lstmap(pipe_tokens, &init_t_cmd, &destroy_tlist_of_tcmd);
	ft_lstclear(&pipe_tokens, free);
	return (cmd_table);
}

static int	init_cmd_table_more( \
	t_list *cmd_table, t_env *env, int *last_ret_val)
{
	t_list	*cmd_table_iter;
	t_cmd	*cmd;

	cmd_table_iter = cmd_table;
	while (cmd_table_iter)
	{
		cmd = (t_cmd *) cmd_table_iter->content;
		if (!cmd->split_token)
		{
			cmd_table_iter = cmd_table_iter->next;
			continue ;
		}
		if (transform_tokens2(&cmd->split_token, last_ret_val) == 1)
		{
			printf("init_cmd_table_more(): A\n");
			return (1);
		}
		if (build_cmd_args(cmd, env) == 1 && cmd->path_error == 0)
		{
			printf("init_cmd_table_more(): B\n");
			return (1);
		}
		cmd_table_iter = cmd_table_iter->next;
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
	t_list	*cmd_table;
	t_list	*cmd_table_iter;
	t_cmd	*cmd;

	cmd_table = create_cmd_table(line);
	if (!cmd_table)
		return (NULL);
	if (parse_redir_loop(cmd_table) != 0)
		return (init_cmd_table_destroyer(&cmd_table));
	cmd_table_iter = cmd_table;
	while (cmd_table_iter)
	{
		cmd = (t_cmd *) cmd_table_iter->content;
		if (transform_tokens1(&cmd->split_token, env, &last_ret_val) == 1)
			return (init_cmd_table_destroyer(&cmd_table));
		if (expand_vars_in_filenames(cmd, env, &last_ret_val) == 1)
			return (init_cmd_table_destroyer(&cmd_table));
		cmd_table_iter = cmd_table_iter->next;
	}
	if (init_cmd_table_more(cmd_table, env, &last_ret_val) == 1)
		return (init_cmd_table_destroyer(&cmd_table));
	return (cmd_table);
}
