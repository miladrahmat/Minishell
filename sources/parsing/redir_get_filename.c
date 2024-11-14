/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_get_filename.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 15:15:01 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/14 13:54:13 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_filename_move_end(char **start, char *end)
{
	while (*end)
	{
		if (*end == '\'')
		{
			end++;
			*start = end;
			end = skip_until(end, '\'');
			break ;
		}
		else if (*end == '\"')
		{
			end++;
			*start = end;
			end = skip_until(end, '\"');
			break ;
		}
		else if (is_whitespace(*end))
			break ;
		else
			end++;
	}
	return (end);
}

// returns NULL only in malloc fail
static char	*get_filename(char *start)
{
	char	*end;
	char	*new_str;
	char	*ret;

	ret = NULL;
	start = skip_whitespace(start);
	end = start;
	while (*start)
	{
		end = get_filename_move_end(&start, end);
		new_str = ft_strndup(start, substr_len(start, end));
		if (!new_str)
			return (NULL);
		ret = ft_strjoin(ret, new_str);
		free(new_str);
		if (!ret)
		{
			free(new_str);
			return (NULL);
		}
		start = end;
	}
	return (ret);
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
