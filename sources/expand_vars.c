/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 15:27:49 by lemercie          #+#    #+#             */
/*   Updated: 2024/10/14 16:59:10 by lemercie         ###   ########.fr       */
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
char	*concatenate_until(char **dst, char *src, char delim)
{
	char	*src_end;
	char	*temp;
	char	*temp_joined;

	src_end = src;
	while (*src_end && *src_end != delim)
	{
		src_end++;
	}
	temp = ft_strndup(src, substr_len(src, src_end));
	if (!temp)
		return (NULL);
	temp_joined = ft_strjoin(*dst, temp);
	if (temp_joined)
		*dst = temp_joined;
	return (src_end);
}

// join until $ or single quote (that is not inside of doubles)
// if $, get word and expand
// if single quote join until next single quote

// creates a new string where $VAR in the token is replaced by the value of
// $VAR in env
// TODO: expand in double quotes but not in single quotes
// TODO: variable name can contain alphanumeric chars and underscore
// 		after that it ends
// 			so: $HOMEA => variable not found
//				$HOMEa => variable not found
//				$HOME- => /home/lemercie-
char	*expand_vars2(char *token, t_env *env)
{
	char	*ret;
	char	*start;
	char	*end;
	char	*varname;
	char	*value;

	start = token;
	end = token;
	ret = NULL;
	while (*start)
	{
		end = concatenate_until(&ret, start, '$');
		if (!ret)
			return (NULL);
	//	if (!*end)
	//		return (ret);
		//TODO: has to NOT be until whitespace but until non alphanum 
		//or underscore
		varname = get_word(end + 1);
		printf("%s\n", varname);
		if (!varname)
		{
			free(ret);
			return (NULL);
		}
		value = ft_env_get_value_by_key(varname, env);
		if (value)
		{
			ret = ft_strjoin(ret, value);
			if (!ret)
			{
				free(varname);
				return (NULL);
			}
		}
		free(varname);
		end = skip_word(end);
		start = end;
	}
	return (ret);
}

char	*expand_vars(char *token, t_env *env)
{
	char	*ret;
	char	*start;
	char	*end;
	char	*varname;
	char	*temp;
	char	*value;

	start = token;
	end = token;
	ret = NULL;
	printf("expand_tokens() before: %s\n", token);
	while (*start)
	{
		while (*end && *end != '$')
			end++;
		temp = ft_strndup(start, substr_len(start, end));
		if (!temp)
		{
			if (ret)
				free (ret);
			return (NULL);
		}
		ret = ft_strjoin(ret, temp);
		free(temp);
		if (!ret)
			return (NULL);
		if (*end != '$')
			break ;
		varname = get_word(end + 1);
		if (!varname)
		{
			free(ret);
			return (NULL);
		}
		value = ft_env_get_value_by_key(varname, env);
		if (value)
		{
			ret = ft_strjoin(ret, value);
			if (!ret)
			{
				free(varname);
				return (NULL);
			}
		}
		free(varname);
		end = skip_word(end);
		start = end;
	}
	return (ret);
}
