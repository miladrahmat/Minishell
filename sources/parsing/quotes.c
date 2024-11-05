/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 15:28:18 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/05 15:28:43 by lemercie         ###   ########.fr       */
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
