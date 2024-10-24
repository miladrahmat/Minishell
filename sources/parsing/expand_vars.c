/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 15:27:49 by lemercie          #+#    #+#             */
/*   Updated: 2024/10/23 12:25:55 by lemercie         ###   ########.fr       */
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

// return pointer to the last char copied from src
// replace dst
// NULL pointer is returned in case of malloc() failure in ft_strndup() or 
// ft_strjoin()
char	*concatenate_until(char **dst, char *src, char *delim)
{
	char	*src_end;
	char	*temp;
	char	*temp_joined;

//	printf("concat delims %c - %c\n", delim[0], delim[1]);
	src_end = src;
	while (*src_end && !ft_strchr(delim, *src_end))
	{
		src_end++;
	}
	temp = ft_strndup(src, substr_len(src, src_end));
	if (!temp)
		return (NULL);
	temp_joined = ft_strjoin(*dst, temp);
	if (!temp_joined)
		return (NULL);
	*dst = temp_joined;
	return (src_end);
}

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

	end = skip_varname(start);
	return (strndup(start, substr_len(start, end)));
}

// returns NULL so that it can be used inside of a return statement
void	*expand_vars_fail(char *s1, char *s2)
{
	if (s1)
		free(s1);
	if (s2)
		free(s2);
	return (NULL);
}

// join until $ or single quote (that is not inside of doubles)
// if $, get word and expand
// if single quote join until next single quote

// creates a new string where $VAR in the token is replaced by the value of
// $VAR in env
// token is NOT freed here because it is contained in a list node in the caller
// returns NULL in case of malloc fails
char	*expand_vars(char *token, t_env *env)
{
	char	*ret;
	char	*start;
	char	*end;
	char	*varname;
	char	*value;

	if (!token)
		return (NULL);
	start = token;
	end = token;
	ret = NULL;
	varname = NULL;
	while (*start)
	{
		end = concatenate_until(&ret, start, "$'");
		if (!end)
			return (expand_vars_fail(ret, varname));
		if (!ret)
		{
			printf("expand_vars() returning NULL\n");
			return (expand_vars_fail(ret, varname));
		}
		if (*end == '$')
		{
			varname = get_varname(end + 1);
			if (!varname)
				return (expand_vars_fail(ret, varname));
			value = ft_env_get_value_by_key(varname, env);
			free(varname);
			if (value)
			{
				ret = ft_strjoin(ret, value);
				if (!ret)
					return (expand_vars_fail(ret, varname));
			}
			end = skip_varname(end);
		}
		else if (*end == '\'')
		{
			end++;
			start = end;
			end = concatenate_until(&ret, start, "'");
			if (!end)
				return (expand_vars_fail(ret, varname));
		}
		start = end;
	}
	if (ft_strlen(ret) <= 0)
		printf("expand_vars() returning empty string\n");
	return (ret);
}
