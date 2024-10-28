/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 10:26:34 by lemercie          #+#    #+#             */
/*   Updated: 2024/10/28 15:14:11 by lemercie         ###   ########.fr       */
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

	/*
	if (start == end)
		printf("get_token(): start and end same\n");
	*/
	len = get_token_len(start, end);
	if (len == 0)
	{
	//	printf("get_token(): len is 0\n");
		return (NULL);
	}
	token = malloc(sizeof(char) * (len + 1));
	ft_strlcpy(token, start, len + 1);
//	printf("get-token() returning token: %s\n", token);
	return (token);
}
