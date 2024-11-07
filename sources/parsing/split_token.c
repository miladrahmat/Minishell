/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 13:22:20 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/06 13:22:43 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// env vars can expand into commands, arguments or redir filenames,
// 		but cannot contain < or > in themselves
// 	==> split on spaces and redir symbols
t_list	*split_token(char *cmd_token)
{
	char	*start;
	char	*end;
	t_list	*new_token;
	t_list	*tokens;
	int		quotes;
	char	*new_str;

	if (!cmd_token)
		return NULL;
	start = cmd_token;
	tokens = NULL;
	end = start;
	quotes = 0;
//	printf("split_token incoming token: %s\n", end);
	while (*start && *end)
	{
		start = skip_whitespace(start);
		end = start;
		if (*end == '<' || *end == '>')
		{
			end++;
			if (*end == '<' || *end == '>')
				end++;
		}
		while (*end)
		{
			if (quotes == 0)
			{
				if (*end == '\'')
				{
					quotes = 1;
				}
				else if (*end == '\"')
				{
					quotes = 2;
				}
				else if (*end == '<' || *end == '>' || is_whitespace(*end))
					break ;
			}
			else if (quotes == 1 && *end == '\'')
			{
				quotes = 0;
			}
			else if (quotes == 2 && *end == '\"')
			{
				quotes = 0;
			}
			end++;
		}
		new_str = get_token(start, end);
		if (new_str)
		{
	//		printf("split_token(): %s\n", new_str);
			new_token = ft_lstnew(new_str);
			ft_lstadd_back(&tokens, new_token);
		}
		start = end;
	}
	return (tokens);
}
