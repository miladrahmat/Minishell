/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 14:02:33 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/10/02 10:55:43 by mrahmat-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	echo_print(char *str, int fd)
{
	while (*str != '\0')
	{
		if (*str == '\'')
			while (*str++ != '\'' && *str != '\0')
				ft_putchar_fd(*str, fd);
		else if (*str == '"')
			while (*str++ != '"' && *str != '\0')
				ft_putchar_fd(*str, fd);
		else
			ft_putchar_fd(*str++, fd);
	}
}

int	echo(char **str, int fd)
{
	int		n;

	n = 0;
	if (ft_strncmp(*str, "-n", ft_strlen(*str)) == 0)
	{
		n = 1;
		str++;
	}
	while (*str != NULL)
	{
		echo_print(*str, fd);
		str++;
		if (*str != NULL)
			ft_putchar_fd(' ', fd);
	}
	if (n == 0)
		ft_putchar_fd('\n', fd);
	return (0);
}
