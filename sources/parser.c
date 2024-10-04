/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 10:11:49 by lemercie          #+#    #+#             */
/*   Updated: 2024/10/04 15:30:33 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
//WEIRD CASES
//	echo stuff > outfile more
//		==> outfile will contain: stuff more
//
// TOKENIZATION
// 	1. check for odd numbered quotes
// 	2. split on pipes (but not pipes inside quotes)
// 	3. transform into command table
// 	4. expand variables (also inside of double quotes)
// 	5. remove quotes
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
	{
		s++;
	}
	return (s);
}

size_t	get_token_len(char *start, char *end)
{
	size_t	len;

	len = 0;
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
	start++;
	end = start;
	while (*end != quote)
	{
		end++;
	}
	*new_token = ft_lstnew(get_token(start, end));
	return (end);
}

// lets make it so start is inclusive and end is not inclusive
t_list	*tokenize(char *line)
{
	char	*start;
	char	*end;
	t_list	*new_token;
	t_list	*tokens;
	int		quotes;

	if (!line)
		return NULL;
	start = line;
	tokens = NULL;
	end = start;
	quotes = 0;
	while (*start && *end)
	{
		start = skip_whitespace(start);
		end = start;
		/*
		if (*start == '\'' || *start == '\"')
		{
			start = get_quoted_string(start, &new_token);
			ft_lstadd_back(&tokens, new_token);
			start++;
			end = start;
			continue ;
		}
		*/
		//while (*end && !is_whitespace(*end))
		while (*end)
		{
			if (quotes == 0)
			{
				if (*end == '\'')
					quotes = 1;
				else if (*end == '\"')
					quotes = 2;
				else if (*end == '|')
					break ;
			}
			else if (quotes == 1 && *end == '\'')
				quotes = 0;
			else if (quotes == 2 && *end == '\"')
				quotes = 0;
			end++;
		}
		new_token = ft_lstnew(get_token(start, end));
		ft_lstadd_back(&tokens, new_token);
		if (*(end + 1))
			end++;
		else
			break ;
		start = end;
	}
	return (tokens);
}
