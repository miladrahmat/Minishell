/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 13:22:20 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/16 17:50:56 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// skips max two arrows
static char	*skip_arrows(char *s)
{	
	if (*s == '<' || *s == '>')
	{
		s++;
		if (*s == '<' || *s == '>')
			s++;
	}
	return (s);
}

// advances until arrow ow whitespace that is not inside of quotes
static char	*find_end(char *s)
{
	int	quotes;

	quotes = 0;
	while (*s)
	{
		if (quotes == 0)
		{
			if (*s == '\'')
				quotes = 1;
			else if (*s == '\"')
				quotes = 2;
			else if (*s == '<' || *s == '>' || is_whitespace(*s))
				break ;
		}
		else if (quotes == 1 && *s == '\'')
			quotes = 0;
		else if (quotes == 2 && *s == '\"')
			quotes = 0;
		s++;
	}
	return (s);
}

// env vars can expand into commands, arguments or redir filenames,
// 		but cannot contain < or > in themselves
// 	==> split on spaces and redir symbols
// 	The start == end check is so that we dont get a NULL from get_token()
// 		unless it's a malloc() fail. 
t_list	*split_token(char *cmd_token)
{
	char	*start;
	char	*end;
	t_list	*new_token;
	t_list	*tokens;
	char	*new_str;

	if (!cmd_token)
		return (NULL);
	start = cmd_token;
	tokens = NULL;
	end = start;
	while (*start && *end)
	{
		start = skip_whitespace(start);
		end = skip_arrows(start);
		end = find_end(end);
		if (start == end)
			continue ;
		new_token = ft_lstnew(get_token(start, end));
		if (!new_token || !new_token->content)
			return (free_strs(&new_str, NULL));
		ft_lstadd_back(&tokens, new_token);
		start = end;
	}
	return (tokens);
}
