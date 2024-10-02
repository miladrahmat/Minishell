/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 10:11:49 by lemercie          #+#    #+#             */
/*   Updated: 2024/10/02 11:57:56 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./minishell.h"

// expand $ env variable
// 	special case $?
//
// REDIRECTIONS
// left arrow has filename on the right
// right arrow has filename on the right
// but they can be on either side of the command
//
// HEREDOC
//
// QUOTES
// first check for invalid input if odd number of quotes
// nested tokenization with quotes first, then spaces?
//
// inside of quotes can be: filename, command name, argument to command
// but not a combination
//
// 		SINGLE QUOTES
// 		no variable expansion
//
// 		DOUBLE QUOTES 
// 		variables are expanded
//
// NOT INCLUDED
// 	variables with equals sign
//
//
// 	struct cmd
// 		infile
// 			stdin
// 			file redirect
// 			heredoc
// 		outfile
// 			stdout
// 			ordinary redirect
// 			append redirect
// 		arguments
//
//
//
bool	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

bool	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\v' ||
	c =='\f');
}

char	*skip_whitespace(char *s)
{
	if (!s)
		return (NULL);
	while (*s && is_whitespace(*s))
		s++;
	return (s);
}

size_t	get_token_len(char *start, char *end)
{
	size_t	len;

	len = 1;
	while (start != end)
	{
		start++;
		len++;
	}
	return (len);
}

char	*get_token(char *start, char *end)
{
	char	*token;
	int		len;

	len = get_token_len(start, end);
	token = malloc(sizeof(char) * (len + 1));
	ft_strlcpy(token, start, len + 1);
	return (token);
}

char	*get_quoted_string(char *start, t_list **new_token)
{
	char	quote;
	char	*end;


	quote = *start;
	end = start + 1;
	while (*end != quote)
	{
		end++;
	}
	*new_token = ft_lstnew(get_token(start, end));
	return (end);
}

t_list	*tokenize(char *line)
{
	char	*start;
	char	*end;
	t_list	*new_token;
	t_list	*tokens;

	if (!line)
		return NULL;
	start = skip_whitespace(line);
	end = start + 1;
	tokens = NULL;
	while (*start && *end)
	{
		if (*start == '\'' || *start == '\"')
		{
			start = get_quoted_string(start, &new_token);
			ft_lstadd_back(&tokens, new_token);
			end = start + 1;
			continue ;
		}
		while (!is_whitespace(*end))
		{
			end++;
		}
		new_token = ft_lstnew(get_token(start, end));
		ft_lstadd_back(&tokens, new_token);
		start = end;
		
	}
	return (tokens);
}
