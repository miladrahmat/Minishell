/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_get_filename.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 15:15:01 by lemercie          #+#    #+#             */
/*   Updated: 2024/12/13 12:08:18 by mrahmat-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_end(char *s)
{
	while (s && *s)
	{
		s++;
	}
	return (s);
}

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
static char	*get_filename(char *start, bool heredoc)
{
	char	*end;
	char	*new_str;
	char	*ret;
	char	*temp;

	int	i = 0;
	ret = NULL;
	start = skip_whitespace(start);
	end = start;
	while (*start)
	{
		printf("looping %d\n", i++);
		if (heredoc)
			end = get_end(end);
		else
			end = get_filename_move_end(&start, end);
		new_str = ft_strndup(start, substr_len(start, end));
		printf("filename: %s\n", new_str);
		if (!new_str)
			return (NULL);
		temp = ret;
		ret = ft_strjoin(ret, new_str);
		free_strs(&temp, &new_str);
		if (!ret)
			free_strs(&new_str, NULL);
		start = end;
	}
	return (ret);
}

// malloc fail returned directly
static char	*get_filename_wrapper_helper(
	int *tokens_consumed, char *tokens[2], unsigned int offset, bool heredoc)
{
	char	*filename;

	filename = NULL;
	if (ft_strlen(tokens[0]) > offset)
	{
		filename = get_filename(tokens[0] + offset, heredoc);
		*tokens_consumed = 1;
	}
	else if (tokens[1])
	{
		filename = get_filename(tokens[1], heredoc);
		*tokens_consumed = 2;
	}
	return (filename);
}

// return 1 on malloc fail
int	get_filename_wrapper(
	t_redir *redir, int *tokens_consumed, char *token1, char *token2)
{
	char	*tokens[2];

	tokens[0] = token1;
	tokens[1] = token2;
	if (redir->redir_type == out_append)
	{
		redir->filename = get_filename_wrapper_helper(
				tokens_consumed, tokens, 2, false);
	}
	else if (redir->redir_type == heredoc)
	{
		redir->filename = get_filename_wrapper_helper(
				tokens_consumed, tokens, 2, true);
	}
	else if (redir->redir_type == out_trunc || redir->redir_type == input)
	{
		redir->filename = get_filename_wrapper_helper(
				tokens_consumed, tokens, 1, false);
	}
	if (!redir->filename)
		return (1);
	return (0);
}
