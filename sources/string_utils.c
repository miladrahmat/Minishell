/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 10:49:45 by lemercie          #+#    #+#             */
/*   Updated: 2024/10/07 14:58:52 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	substr_len(char *start, char *end)
{
	int	i;
	i = 0;
	while (start != end)
	{
		start++;
		i++;
	}
	return (i);
}

char	*get_word(char *start)
{
	char	*end;

	start = skip_whitespace(start);
	end = start;
	while (!is_whitespace(*end))
	{
		end++;
	}
	return (strndup(start, substr_len(start, end)));
}