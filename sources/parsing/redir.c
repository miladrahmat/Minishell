/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 15:23:14 by lemercie          #+#    #+#             */
/*   Updated: 2024/12/03 17:12:46 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// > output
// >> output append
// < input
// << input heredoc
static t_redir_type	get_redir_type(char *content)
{
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
static int	get_redir(t_cmd *cmd, char *token1, char *token2)
{
	t_redir	*redir;
	t_list	*new_node;
	int		tokens_consumed;

	tokens_consumed = 0;
	if (get_redir_type(token1) == error)
		return (0);
	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (-1);
	redir->heredoc_quoted_delim = false;
	redir->redir_type = get_redir_type(token1);
	if (get_filename_wrapper(redir, &tokens_consumed, token1, token2) == 1)
		return (get_redir_destroyer(redir));
	if (redir->redir_type == heredoc)
		check_quoted_heredoc_delim(redir);
	if (is_quoted_str(redir->filename))
		str_del_first_last(redir->filename);
	new_node = ft_lstnew(redir);
	if (!new_node)
		return (get_redir_destroyer(redir));
	ft_lstadd_back(&cmd->files, new_node);
	return (tokens_consumed);
}

static void	delete_tokens(t_list **head, t_list **to_del, int n)
{
	while (n > 0)
	{
		ft_lstdel_and_connect(head, to_del);
		n--;
	}
}

static int	parse_redirs(t_cmd *cmd)
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
		printf("parse_redirs(): before del: %i\n", ft_lstsize(cmd->split_token));
		printf("parse_redirs(): %s\n", (char *) tokens_iter->content);
		delete_tokens(&cmd->split_token, &tokens_iter, tokens_consumed);
		printf("parse_redirs(): after del: %i\n", ft_lstsize(cmd->split_token));
		if (!tokens_iter)
		{
			printf("break\n");
			break ;
		}
		tokens_iter = tokens_iter->next;
	}
	return (0);
}

int	parse_redir_loop(t_list *cmd_table)
{
	t_list	*cmd_table_iter;
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
