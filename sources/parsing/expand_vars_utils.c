/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 14:35:57 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/26 14:38:22 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	stupid_join(char **s1, char *s2, bool s2_alloc)
{
	char	*temp;

	temp = *s1;
	*s1 = ft_strjoin(*s1, s2);
	free(temp);
	if (s2_alloc)
		free(s2);
	if (!*s1)
		return (1);
	return (0);
}

// return pointer to the last char copied from src
// replace dst
// NULL pointer is returned in case of malloc() failure in ft_strndup() or 
// ft_strjoin()
char	*concatenate_until(char **dst, char *src, char *delim)
{
	char	*src_end;
	char	*new_str;
	size_t	quote;

	if (*delim == '\'')
	{
		quote = skip_quotes(src);
		new_str = ft_substr(src, 1, quote);
		src_end = src + quote;
		if (src[quote] != '\0')
			src_end++;
	}
	else
	{
		src_end = src;
		while (*src_end && !ft_strchr(delim, *src_end))
			src_end++;
		new_str = ft_strndup(src, substr_len(src, src_end));
	}
	if (!new_str)
		return (NULL);
	if (stupid_join(dst, new_str, true) > 0)
		return (NULL);
	return (src_end);
}
