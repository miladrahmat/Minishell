/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 15:27:49 by lemercie          #+#    #+#             */
/*   Updated: 2024/10/10 17:24:04 by lemercie         ###   ########.fr       */
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

char	*get_var_value(char *key, t_list *env)
{
	t_list	*match;

	match = get_key_value(key);
	return (((t_env *)match->content)->value);
}

// TODO: create helper function skip_word()
char	*expand_vars(char *token, t_list *env)
{
	char	*ret;
	char	*start;
	char	*end;

	start = token;
	end = token;
	while (*start)
	{
		while (*end && *end != '$')
		{
			end++;
		}
		ret = ft_strjoin(ret, ft_strndup(start, substr_len(start, end)));
		if (*end == '$')
		{
			ret = ft_strjoin(ret, get_var_value(end + 1, env));
			while (*end && !is_whitespace(*end))
			{
				end++;
			}
			while (*end && is_whitespace(*end))
			{
				end++;
			}
			start = end;
		}
		else
			break ;
	}

	// strjoin(strndup(substr_len)))
	// strlcpy until $
	// strjoin value of env variable
	// skip env word
	
	return (ret);
}
