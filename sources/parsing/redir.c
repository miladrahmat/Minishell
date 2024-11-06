/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 15:23:14 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/06 13:31:58 by lemercie         ###   ########.fr       */
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
	if (!content || !*content)
		return (error);
	if (content[0] == '>')
	{
		if (content[1] == '>')
		{
			return (out_append);
		}
		else
			return (out_trunc);
	}
	else if (content[0] == '<')
	{
		if (content[1] == '<')
		{
			return (heredoc);
		}
		else
			return (input);
	}
	return (error);
}

// return amount of tokens consumed?
// TODO: "cat <<" should be syntax error
int	get_redir(t_cmd *cmd, char *token1, char *token2)
{
	t_redir	*redir;
	t_list	*new_node;
	int		tokens_consumed;

//	printf("get_redir(): token1: %s, token2: %s\n", token1, token2);
	if (get_redir_type(token1) == error)
	{
//		printf("get_redir(): error in get_redir_type()\n");
		return (0);
	}
	redir = malloc(sizeof(t_redir));
	
	if (!redir)
		return (1);
	redir->heredoc_quoted_delim = false;
	redir->redir_type = get_redir_type(token1);
	if (redir->redir_type == out_append)
	{
		if (ft_strlen(token1) > 2)
		{
			redir->filename = get_word_quote(token1 + 2);
			tokens_consumed = 1;
		}
		else
		{
			if (token2)
			{
				redir->filename = ft_strdup(token2);
				tokens_consumed = 2;
			}
		}
	}
	else if (redir->redir_type == out_trunc || redir->redir_type == input)
	{
		if (ft_strlen(token1) > 1)
		{
			redir->filename = get_word_quote(token1 + 1);
			tokens_consumed = 1;
		}
		else
		{
			if (token2)
			{
				redir->filename = ft_strdup(token2);
				tokens_consumed = 2;
			}
		}
	}
	else if (redir->redir_type == heredoc)
	{
		if (ft_strlen(token1) > 2)
		{
			redir->filename = get_word_quote(token1 + 2);
			tokens_consumed = 1;
		}
		else
		{
			if (token2)
			{
				redir->filename = ft_strdup(token2);
				tokens_consumed = 2;
			}
		}
		check_quoted_heredoc_delim(redir);
//		printf("get_redir(): heredoc delim: %s\n", redir->filename);
	}
//	printf("get_redir(): %s\n", redir->filename);
	if (is_quoted_str(redir->filename))
		str_del_first_last(redir->filename);
//	printf("get_redir(): %s\n", redir->filename);
	new_node = ft_lstnew(redir);
	if (!new_node || !redir->filename)
		return (1);
	ft_lstadd_back(&cmd->files, new_node);
	return (tokens_consumed);
}

void	parse_redirs(t_cmd *cmd)
{
	t_list	*tokens;
	t_list	*tokens_iter;
	int		tokens_consumed;

//	printf("parse_redirs()\n");
	tokens = cmd->split_token;
	tokens_iter = tokens;
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
		if (tokens_consumed > 0)
		{
			ft_lstdel_and_connect(&cmd->split_token, &tokens_iter);
		}
		if (tokens_consumed == 2)
		{
			tokens_iter = tokens_iter->next;
			if (tokens_iter)
			{
				ft_lstdel_and_connect(&cmd->split_token, &tokens_iter);
			}
		}
		if (!tokens_iter)
			return ;
		tokens_iter = tokens_iter->next;
	}
}

void	parse_redir_loop(void *arg)
{
	parse_redirs((t_cmd *) arg);
}
