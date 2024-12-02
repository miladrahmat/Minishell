/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils_words.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 15:21:02 by lemercie          #+#    #+#             */
/*   Updated: 2024/12/02 11:59:53 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	while (*end && !is_whitespace(*end))
	{
		end++;
	}
	return (ft_strndup(start, substr_len(start, end)));
}

// malloc fails returned directly
char	*get_word_quote(char *start)
{
	char	*end;

	start = skip_whitespace(start);
	if (*start == '\'')
	{
		start++;
		end = start;
		end = skip_until(end, '\'');
		return (ft_strndup(start, substr_len(start, end)));
	}
	if (*start == '\"')
	{
		start++;
		end = start;
		end = skip_until(end, '\"');
		return (ft_strndup(start, substr_len(start, end)));
	}
	end = start;
	while (*end && !is_whitespace(*end))
	{
		end++;
	}
	return (ft_strndup(start, substr_len(start, end)));
}
