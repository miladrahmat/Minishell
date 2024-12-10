/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 09:58:07 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/12/10 11:05:28 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	skip_quotes(char *str)
{
	size_t	i;

	i = 0;
	if (str[i] == '\'')
	{
		i++;
		while (str[i] != '\'' && str[i] != '\0')
			i++;
	}
	else if (str[i] == '\"')
	{
		i++;
		while (str[i] != '\"' && str[i] != '\0')
			i++;
	}
	return (i);
}

int	print_syntax_error(char token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	if (token == '\0')
		ft_putstr_fd("newline", 2);
	ft_putchar_fd(token, 2);
	ft_putchar_fd('\'', 2);
	ft_putchar_fd('\n', 2);
	return (-1);
}

int	check_whitespace(char *str)
{
	size_t	i;

	i = 0;
	while (is_whitespace(str[i]))
		i++;
	if (str[i] == '\0')
		return (-1);
	return (1);
}

int	check_error_redir(char *str, char redir_type, int *redir_num, bool *is)
{
	char	*special_chars;

	special_chars = get_special_chars(redir_type);
	if (*str == redir_type)
	{
		*is = true;
		if (*redir_num > 0 && *(str - 1) != redir_type)
			return (-1);
		(*redir_num)++;
		if (*redir_num > 2)
			return (-1);
	}
	else if (*str == '|' && *is == false)
		*redir_num = 0;
	if (*is == true && ft_strchr(special_chars, *str) != 0)
		return (-1);
	else if (ft_isprint(*str) && *is && *str != '|' && *str != redir_type \
			&& !is_whitespace(*str))
	{
		*redir_num = 0;
		*is = false;
	}
	if (*is && (*str == '\0' || str[1] == '\0'))
		return (-1);
	return (1);
}

int	check_error_pipe(char *str, bool *is)
{
	if (*str == '|' && *is == false)
		*is = true;
	else if (*is == true && *str == '|')
		return (-1);
	if (ft_isprint(*str) && *is == true && *str != '|' \
		&& !is_whitespace(*str))
		*is = false;
	if ((str[1] == '\0' || str[1] == '|') && *is == true)
		return (-1);
	return (1);
}
