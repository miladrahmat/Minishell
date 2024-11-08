/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_on_pipes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 10:11:49 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/08 10:30:18 by lemercie         ###   ########.fr       */
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
// TODO: handle malloc fail in get_token() and ft_lstnew()
t_list	*split_on_pipes(char *line)
{
	char	*start;
	char	*end;
	t_list	*new_token;
	t_list	*tokens;
	char	*new_string;

	if (!line)
		return NULL;
	start = line;
	tokens = NULL;
	end = start;
	while (*start && *end)
	{
		if (*start == '|')
			start++;
		start = skip_whitespace(start);
		end = start;
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
		new_string = get_token(start, end);
		if (!new_string)
			return (NULL);
		new_token = ft_lstnew(new_string);
		if (!new_token)
		{
			free(new_string);
			return (NULL);
		}
		ft_lstadd_back(&tokens, new_token);
		start = end;
	}
	return (tokens);
}
