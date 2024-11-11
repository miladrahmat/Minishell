/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 15:23:14 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/11 15:38:30 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_quoted_heredoc_delim(t_redir *redir)
{
	if (is_double_quoted_str(redir->filename))
	{
		str_del_first_last(redir->filename);
		redir->heredoc_quoted_delim = true;
	}
}

// > output
// >> output append
// < input
// << input heredoc
t_redir_type	get_redir_type(char *content)
{
//	printf("get_redir_type(): incoming content: %s\n", content);
	if (!content || !(*content))
		return (error);
	if (ft_strlen(content) <= 0)
		return (error);
	if (content[0] == '>')
	{
		if (content[1] == '>')
			return (out_append);
		else
			return (out_trunc);
	}
	else if (content[0] == '<')
	{
		if (content[1] == '<')
			return (heredoc);
		else
			return (input);
	}
	return (error);
}

// return amount of tokens consumed and negative number on error
// TODO: "cat <<" should be syntax error
int	get_redir(t_cmd *cmd, char *token1, char *token2)
{
	t_redir	*redir;
	t_list	*new_node;
	int		tokens_consumed;

	tokens_consumed = 0;
//	printf("get_redir(): token1: %s, token2: %s\n", token1, token2);
	if (get_redir_type(token1) == error)
		return (0);
	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (-1);
	redir->heredoc_quoted_delim = false;
	redir->redir_type = get_redir_type(token1);
	if (redir->redir_type == out_append)
	{
		if (ft_strlen(token1) > 2)
		{
			redir->filename = get_filename(token1 + 2);
			tokens_consumed = 1;
		}
		else if (token2)
		{
			redir->filename = ft_strdup(token2);
			tokens_consumed = 2;
		}
	}
	else if (redir->redir_type == out_trunc || redir->redir_type == input)
	{
		if (ft_strlen(token1) > 1)
		{
			redir->filename = get_filename(token1 + 1);
			tokens_consumed = 1;
		}
		else if (token2)
		{
			redir->filename = ft_strdup(token2);
			tokens_consumed = 2;
		}
	}
	else if (redir->redir_type == heredoc)
	{
		if (ft_strlen(token1) > 2)
		{
			redir->filename = get_filename(token1 + 2);
			tokens_consumed = 1;
		}
		else if (token2)
		{
			redir->filename = ft_strdup(token2);
			tokens_consumed = 2;
		}
		check_quoted_heredoc_delim(redir);
	}
	if (!redir->filename)
	{
		free(redir);
		return (-1);
	}
	if (is_quoted_str(redir->filename))
		str_del_first_last(redir->filename);
	new_node = ft_lstnew(redir);
	if (!new_node)
	{
		free(redir->filename);
		free(redir);
		return (-1);
	}
//	printf("get_redir: %s\n", redir->filename);
	ft_lstadd_back(&cmd->files, new_node);
	return (tokens_consumed);
}

int	parse_redirs(t_cmd *cmd)
{
	t_list	*tokens_iter;
	int		tokens_consumed;

	tokens_iter = cmd->split_token;
	while (tokens_iter)
	{
		tokens_consumed = 0;
		if (tokens_iter->next)
		{
			tokens_consumed = get_redir(cmd, tokens_iter->content,
				tokens_iter->next->content);
		}
		else
			tokens_consumed = get_redir(cmd, tokens_iter->content, NULL);
		if (tokens_consumed < 0)
			return (1);
		if (tokens_consumed > 0)
			ft_lstdel_and_connect(&cmd->split_token, &tokens_iter);
		if (tokens_consumed == 2)
		{
			tokens_iter = tokens_iter->next;
			if (tokens_iter)
				ft_lstdel_and_connect(&cmd->split_token, &tokens_iter);
		}
		if (!tokens_iter)
			break ;
		tokens_iter = tokens_iter->next;
	}
	return (0);
}

int	parse_redir_loop(t_list *cmd_table)
{
	t_list *cmd_table_iter;
	int		err;

	err = 0;
	cmd_table_iter = cmd_table;
	while (cmd_table_iter)
	{
		err = parse_redirs((t_cmd *) cmd_table_iter->content);
		if (err)
			return (err);
		cmd_table_iter = cmd_table_iter->next;
	}
	return (0);
}
