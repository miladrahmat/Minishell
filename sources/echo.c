/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 14:02:33 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/10/10 16:13:45 by mrahmat-         ###   ########.fr       */
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
		echo_print(*str, fd);
		str++;
		if (*str != NULL)
			ft_putchar_fd(' ', fd);
	}
	if (n == 0)
		ft_putchar_fd('\n', fd);
	return (0);
}
