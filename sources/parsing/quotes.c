/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 15:28:18 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/06 15:46:04 by mrahmat-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_quoted_str(char *s)
{
	if (s)
	{
		return ((s[0] == '\"' && s[ft_strlen(s) - 1] == '\"') ||
			(s[0] == '\'' && s[ft_strlen(s) - 1] == '\'')); 
	}
	return (false);
}

ssize_t	handle_quotes(char *new, char *org, char quote)
{
	size_t	new_i;
	size_t	org_i;
	size_t	ret_val;

	new_i = 0;
	org_i = 0;
	ret_val = 0;
	while (org[org_i] != quote && org[org_i] != '\0')
	{
		new[new_i] = org[org_i];
		new_i++;
		org_i++;
		ret_val++;
	}
	if (org[org_i] != quote)
		return (-1);
	return (ret_val);
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
