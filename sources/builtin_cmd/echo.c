/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 14:02:33 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/10/22 16:44:05 by mrahmat-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	echo_n(char *str)
{
	size_t	i;

	i = 0;
	if (str[i] == '-')
		i++;
	while (str[i] == 'n')
		i++;
	if (str[i] != '\0')
		return (0);
	else
		return (1);
}

int	echo(char **str, int fd)
{
	int		n;

	n = 0;
	if (*str == NULL)
		return (0);
	if (ft_strncmp(*str, "-n", 2) == 0)
	{
		if (echo_n(*str) == 1)
		{
			str++;
			n = 1;
		}
	}
	while (*str != NULL)
	{
		ft_putstr_fd(*str, fd);
		str++;
		if (*str != NULL)
			ft_putchar_fd(' ', fd);
	}
	if (n == 0)
		ft_putchar_fd('\n', fd);
	return (0);
}
