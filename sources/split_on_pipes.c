/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_on_pipes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 10:11:49 by lemercie          #+#    #+#             */
/*   Updated: 2024/10/17 15:38:47 by lemercie         ###   ########.fr       */
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

// basically splits the argument on pipes, taking quotes into account
t_list	*split_on_pipes(char *line)
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
		if (*start == '|')
			start++;
		start = skip_whitespace(start);
		end = start + 1;
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
		start = end;
	}
	return (tokens);
}
