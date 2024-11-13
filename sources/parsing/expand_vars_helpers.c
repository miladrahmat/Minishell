/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars_helpers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 14:18:23 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/13 14:18:52 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*skip_varname(char *s)
{
	if (!s)
		return (NULL);
	if (*s == '$')
		s++;
	while (ft_isalnum(*s) || *s == '_')
	{
		s++;
	}
	return (s);
}

char	*get_varname(char *start)
{
	char	*end;

	if (start[0] == '?')
	{
		return (ft_strdup("?"));
	}
	end = skip_varname(start);
	return (ft_strndup(start, substr_len(start, end)));
}

bool	is_varname(char c)
{
	return (ft_isalnum(c) || c == '_' || c == '?');
}

int	not_varname(char **start, char **end, char **ret)
{
	(*end)++;
	if (!(**end))
		*ret = ft_strjoin(*ret, "$");
	else if (!**end || is_whitespace(**end) || **end == '\"')
	{
		if (!(*end)[1] || is_whitespace((*end)[1]))
			*ret = ft_strjoin(*ret, "$");
	}
	*start = *end;
	return (1);
}
