/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 15:27:49 by lemercie          #+#    #+#             */
/*   Updated: 2024/10/14 11:48:27 by lemercie         ###   ########.fr       */
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

// creates a new string where $VAR in the token is replaced by the value of
// $VAR in env
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
		}
		free(varname);
		end = skip_word(end);
		start = end;
	}
	return (ret);
}
