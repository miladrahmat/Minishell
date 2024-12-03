/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_key_value.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 16:10:53 by lemercie          #+#    #+#             */
/*   Updated: 2024/12/03 15:12:00 by mrahmat-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_valid_key(char *str, char *key, size_t cut)
{
	size_t	check;

	check = 0;
	while (check < cut)
	{
		if ((ft_isalnum(str[check]) == 0 && str[check] != '_' \
			&& str[cut - 1] != '+') || (str[0] >= '0' && str[0] <= '9'))
		{
			print_export_error(&key, true);
			return (false);
		}
		check++;
	}
	return (true);
}

t_env	*set_key_value(char *str)
{
	t_env	*node;
	char	*key;
	char	*value;
	size_t	cut;

	cut = ft_strlen_eq(str);
	if (cut == 0)
		return (NULL);
	if (str[cut - 1] == '+')
		key = ft_substr(str, 0, cut - 1);
	else
		key = ft_substr(str, 0, cut);
	if (key == NULL || key[0] == '\0')
		return (print_export_error(&key, true));
	if (!is_valid_key(str, key, cut))
		return (NULL);
	value = ft_substr(str, cut + 1, ft_strlen(str) - cut);
	node = ft_envnew(key, value);
	return (node);
}
