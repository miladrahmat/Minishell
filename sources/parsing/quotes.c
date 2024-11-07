/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 15:28:18 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/07 15:45:26 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_quoted_str(char *s)
{
	if (s)
	{
		return ((s[0] == '\"' && s[ft_strlen(s) - 1] == '\"') || \
			(s[0] == '\'' && s[ft_strlen(s) - 1] == '\''));
	}
	return (false);
}

int	handle_quotes(char *new, char *org, size_t *new_i, ssize_t *org_i)
{
	char	quote;

	quote = org[(*org_i)];
	(*org_i)++;
	while (org[(*org_i)] != quote && org[(*org_i)] != '\0')
	{
		new[(*new_i)] = org[(*org_i)];
		(*new_i)++;
		(*org_i)++;
	}
	if (org[(*org_i)] != quote)
		return (-1);
	return (1);
}

bool	is_double_quoted_str(char *s)
{
	if (s)
	{
		return ((s[0] == '\"' && s[ft_strlen(s) - 1] == '\"'));
	}
	return (false);
}

void	str_del_first_last(char *s)
{
	int	i;
	int	len;

	len = ft_strlen(s);
	i = 0;
	while (s[i] && s[i + 1])
	{
		s[i] = s[i + 1];
		i++;
	}
	s[len - 2] = 0;
}

char	*strip_quotes(char *s, int *ret_val)
{
	char	*ret;
	size_t	ret_i;
	ssize_t	s_i;

	ret_i = 0;
	s_i = -1;
	ret = malloc((ft_strlen(s) + 1) * sizeof(char));
	if (ret == NULL)
		return (NULL);
//		return (free_strs(s, NULL));
	while ((s)[++s_i] != '\0')
	{
		if ((s)[s_i] == '\'' || (s)[s_i] == '\"')
		{
			if (handle_quotes(ret, s, &ret_i, &s_i) < 0)
			{
				*ret_val = 2;
				return (NULL);
//				return (free_strs(s, &ret));
			}
		}
		else
			ret[ret_i++] = (s)[s_i];
	}
//	free(*s);
	ret[ret_i] = '\0';
	return (ret);
}
