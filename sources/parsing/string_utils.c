/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 10:49:45 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/07 12:45:06 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strndup(const char *s1, size_t len)
{
	char			*s2;
	unsigned int	i;

	i = 0;
	s2 = (char *)malloc(len * sizeof(char) + 1);
	if (s2 == NULL)
		return (NULL);
	i = 0;
	while (i < len && s1[i] != '\0')
	{
		s2[i] = s1[i];
		i++;
	}
	s2[i] = '\0';
	return (s2);
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

char	*skip_word(char *s)
{
	if (!s)
		return (NULL);
	s = skip_whitespace(s);
	while (*s && !is_whitespace(*s))
	{
		s++;
	}
//	s = skip_whitespace(s);
	return (s);
}
