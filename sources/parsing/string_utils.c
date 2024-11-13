/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 10:49:45 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/13 15:24:10 by lemercie         ###   ########.fr       */
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

char	*skip_until(char *s, char delim)
{
	while (s && *s && *s != delim)
	{
		s++;
	}
	return (s);
}

bool	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\v'
		|| c == '\f');
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

char	*ft_strjoin_safe(char const *s1, char const *s2)
{
	if (!s2)
		return (ft_strdup(s1));
	return (ft_strjoin(s1, s2));
}
