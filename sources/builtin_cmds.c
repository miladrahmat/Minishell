/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 12:20:32 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/10/01 11:47:05 by mrahmat-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	pwd(char **cmd, int fd)
{
	char	*res;

	if (ft_strncmp(*cmd, "pwd", ft_strlen(*cmd)) != 0)
	{
		ft_putendl_fd("pwd: too many arguments", 2);
		return (-1);
	}
	res = malloc(1000 * sizeof(char));
	if (res == NULL)
	{
		perror("Malloc error");
		return (-1);
	}
	getcwd(res, 1000);
	if (errno != 0)
	{
		print_error("pwd", strerror(errno));
		errno = 0;
		return (-1);
	}
	ft_putendl_fd(res, fd);
	free(res);
	return (1);
}

int	cd(char **cmd)
{
	chdir(*cmd);
	if (errno != 0)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(*cmd, 2);
		errno = 0;
		return (-1);
	}
	return (3);
}

void	echo_print(char *str, int fd)
{
	while (*str != '\0')
	{
		if (*str == '\'')
			while (*str++ != '\'')
				ft_putchar_fd(*str, fd);
		else if (*str == '"')
			while (*str++ != '"')
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
	return (1);
}

int	check_builtin_cmd(char **cmd, int fd)
{
	if (ft_strncmp(*cmd, "pwd", 3) == 0)
		return (pwd(cmd, fd));
	else if (ft_strncmp("cd", *cmd, 2) == 0)
		return (cd(cmd + 1));
	else if (ft_strncmp(*cmd, "echo", 4) == 0)
		return (echo(cmd + 1, fd));
	return (1);
}

int	main(int ac, char **av)
{
	if (check_builtin_cmd(av + 1, ft_atoi(av[ac - 1])) == -1)
		return (1);
	return (0);
}
