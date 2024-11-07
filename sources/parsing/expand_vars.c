/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 15:27:49 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/07 12:45:49 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
//	src_end = src + 1;
//	printf("concatenate_until(): %s\n", src);
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

	if (start[0] == '?')
	{
		return (ft_strdup("?"));
	}
	end = skip_varname(start);
	return (ft_strndup(start, substr_len(start, end)));
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

bool	is_varname(char c)
{
	return (ft_isalnum(c) || c == '_' || c == '?');
}

// join until $ or single quote (that is not inside of doubles)
// if $, get word and expand
// if single quote join until next single quote

// creates a new string where $VAR in the token is replaced by the value of
// $VAR in env
// token is NOT freed here because it is contained in a list node in the caller
// returns NULL in case of malloc fails
char	*expand_vars(char *token, t_env *env, int *last_ret_val)
{
	char	*ret;
	char	*start;
	char	*end;
	char	*varname;
	char	*value;

	int	tmp_i = 0;
//	printf("expand_vars(): incoming token %s\n", token);
	if (!token)
		return (NULL);
	if (*token == '\'')
	{
	//	printf("expand_vars(): returning single quoted string\n");
		return (ft_strdup(token));
	}
	start = token;
	end = token;
	ret = NULL;
	varname = NULL;
	while (*start)
	{
		if (tmp_i > 100)
		{
			printf("inf loop\n");
			exit(1);
		}
		end = concatenate_until(&ret, start, "$'");
	//	printf("after first concat: %s\n", end);
		if (!end)
			return (expand_vars_fail(ret, varname));
		if (!ret)
		{
			printf("expand_vars() returning NULL\n");
			return (expand_vars_fail(ret, varname));
		}
		if (*end == '$')
		{
			if (!is_varname(*(end + 1)))
			{
				ret = ft_strjoin(ret, "$");
				end++;
				start = end;
				continue;
			}
			varname = get_varname(end + 1);
			if (!varname)
				return (expand_vars_fail(ret, varname));
			if (ft_strcmp(varname, "?") == 0)
			{
				value = ft_itoa(*last_ret_val);
				if (!value)
					return (expand_vars_fail(ret, varname));
				end = end + 2;
			}
			else
			{
				value = ft_env_get_value_by_key(varname, env);
				end = skip_varname(end);
			}
			if (value)
			{
				ret = ft_strjoin(ret, value);
				if (!ret)
					return (expand_vars_fail(ret, varname));
			}
			free(varname);
		}
		else if (*end == '\'')
		{
			ret = ft_strjoin(ret, "'");
			if (!ret)
				return (expand_vars_fail(ret, varname));
			end = concatenate_until(&ret, end + 1, "'");
			if (!end)
				return (expand_vars_fail(ret, varname));
		}
		start = end;
		tmp_i++;
	}
	 //if (ft_strlen(ret) <= 0)
	 	//printf("expand_vars() returning empty string\n");
//	printf("expand_token(): returning: %s\n", ret);
	return (ret);
}
