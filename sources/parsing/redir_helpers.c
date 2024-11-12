/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 15:32:24 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/12 15:37:51 by lemercie         ###   ########.fr       */
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

// malloc fail returned directly
static char	*get_filename_wrapper_helper(
	int *tokens_consumed, char *token1, char *token2, unsigned int offset)
{
	char	*filename;

	filename = NULL;
	if (ft_strlen(token1) > offset)
	{
		filename = get_filename(token1 + offset);
		*tokens_consumed = 1;
	}
	else if (token2)
	{
		filename = ft_strdup(token2);
		*tokens_consumed = 2;
	}
	return (filename);
}

// return 1 on malloc fail
int	get_filename_wrapper(
	t_redir *redir, int *tokens_consumed, char *token1, char *token2)
{
	if (redir->redir_type == out_append || redir->redir_type == heredoc)
	{
		redir->filename = get_filename_wrapper_helper(
				tokens_consumed, token1, token2, 2);
	}
	else if (redir->redir_type == out_trunc || redir->redir_type == input)
	{
		redir->filename = get_filename_wrapper_helper(
				tokens_consumed, token1, token2, 1);
	}
	if (!redir->filename)
		return (1);
	return (0);
}

int	get_redir_destroyer(t_redir *redir)
{
	free(redir->filename);
	free(redir);
	return (-1);
}
