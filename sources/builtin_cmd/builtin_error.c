/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 11:46:02 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/11/13 15:20:14 by mrahmat-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	print_builtin_error(char *cmd, char *arg, char *err, bool alloc)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	if (arg != NULL)
	{
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": ", 2);
	}
	if (err != NULL)
		ft_putendl_fd(err, 2);
	else
		ft_putendl_fd(strerror(errno), 2);
	if (alloc == true)
		free(arg);
	errno = 0;
	return (1);
}

t_env	*print_export_error(char **variable)
{
	if (*variable == NULL)
		return (NULL);
	ft_putstr_fd("minishell: export: ", 2);
	ft_putchar_fd('`', 2);
	if (*variable[0] == '\0')
		ft_putstr_fd("=", 2);
	else
		ft_putstr_fd(*variable, 2);
	ft_putstr_fd("\'", 2);
	ft_putstr_fd(": not a valid identifier\n", 2);
	free(*variable);
	return (NULL);
}

int	validate_str(char *str, char *acc_values)
{
	size_t	s_i;
	size_t	a_i;

	s_i = 0;
	if (str == NULL || acc_values == NULL)
		return (-1);
	while (str[s_i] != '\0')
	{
		a_i = 0;
		while (acc_values[a_i] != '\0')
		{
			if (str[s_i] == acc_values[a_i])
				break ;
			a_i++;
			if (acc_values[a_i] == '\0')
				return (-1);
		}
		s_i++;
	}
	return (1);
}

int	is_overflown(char *str)
{
	long long	nbr;
	size_t		len;

	if (ft_strcmp("-9223372036854775808", str) == 0)
		return (1);
	nbr = ft_atoll(str);
	len = ft_strlen(str);
	if ((nbr == 0 && len != 1) || (nbr == -1 && len != 2))
		return (-1);
	return (1);
}

long long	exit_error_check(char **cmd)
{
	long long	ret_val;

	ret_val = LLONG_MIN;
	if (cmd[1] != NULL && cmd[2] != NULL)
	{
		print_builtin_error("exit", NULL, "too many arguments", false);
		ret_val = 1;
	}
	else if (cmd[1] != NULL)
	{
		if (validate_str(cmd[1], "-+0123456789") == -1 \
			|| is_overflown(cmd[1]) < 0)
		{
			print_builtin_error("exit", cmd[1], "numeric argument required", \
				false);
			ret_val = 2;
		}
		else
			ret_val = ft_atoll(cmd[1]);
	}
	return (ret_val);
}
