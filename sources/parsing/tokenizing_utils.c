/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 10:26:34 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/07 17:10:35 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	get_token_len(char *start, char *end)
{
	size_t	len;

	len = 0;
	if (!*start)
		return (0);
	while (start != end)
	{
		start++;
		len++;
	}
	return (len);
}

char	*get_token(char *start, char *end)
{
	char	*token;
	int		len;

	len = get_token_len(start, end);
	if (len == 0)
		return (NULL);
	token = malloc(sizeof(char) * (len + 1));
	if (!token)
		return (NULL);
	ft_strlcpy(token, start, len + 1);
	return (token);
}
