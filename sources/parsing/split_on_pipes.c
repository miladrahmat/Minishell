/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_on_pipes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 10:11:49 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/11 16:53:53 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*move_end(char *end)
{
	while (*end)
	{
		if (*end == '|')
			break ;
		else if (*end == '\'')
			end = skip_until(end + 1, '\'');
		else if (*end == '\"')
			end = skip_until(end + 1, '\"');
		end++;
	}
	return (end);
}

static int	add_token(char *start, char *end, t_list **tokens)
{
	char	*new_string;
	t_list	*new_token;

	new_string = get_token(start, end);
	if (!new_string)
		return (1);
	new_token = ft_lstnew(new_string);
	if (!new_token)
	{
		free(new_string);
		return (1);
	}
	ft_lstadd_back(tokens, new_token);
	return (0);
}

// basically splits the argument on pipes, taking quotes into account
t_list	*split_on_pipes(char *line)
{
	char	*start;
	char	*end;
	t_list	*tokens;

	if (!line)
		return (NULL);
	tokens = NULL;
	start = line;
	end = start;
	while (*start && *end)
	{
		if (*start == '|')
			start++;
		start = skip_whitespace(start);
		end = move_end(start);
		if (add_token(start, end, &tokens) == 1)
			return (NULL);
		start = end;
	}
	return (tokens);
}
