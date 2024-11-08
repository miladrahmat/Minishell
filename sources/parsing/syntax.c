/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 15:53:41 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/11/08 12:34:56 by mrahmat-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_quotes(char *line)
{
	size_t	i;

	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '\'')
		{
			i++;
			while (line[i] != '\'' && line[i] != '\0')
				i++;
			if (line[i] == '\0')
				return (print_syntax_error(line[i]));
		}
		if (line[i] == '\"')
		{
			i++;
			while (line[i] != '\"' && line[i] != '\0')
				i++;
			if (line[i] == '\0')
				return (print_syntax_error(line[i]));
		}
		i++;
	}
	return (1);
}

static int	check_redirect_out(char *line)
{
	size_t	i;
	size_t	check;
	int		redir_num;
	bool	redir;

	i = 0;
	redir_num = 0;
	redir = false;
	while (line[i] != '\0')
	{
		check = skip_quotes(&line[i]);
		if (check != 0)
		{
			i += check;
			redir = false;
		}
		if (check_error_redir(&line[i], '>', &redir_num, &redir) < 0)
			return (print_syntax_error('>'));
		i++;
	}
	return (1);
}

static int	check_redirect_in(char *line)
{
	size_t	i;
	size_t	check;
	int		redir_num;
	bool	redir;

	i = 0;
	redir_num = 0;
	redir = false;
	while (line[i] != '\0')
	{
		check = skip_quotes(&line[i]);
		if (check != 0)
		{
			i += check;
			redir = false;
		}
		if (check_error_redir(&line[i], '<', &redir_num, &redir) < 0)
			return (print_syntax_error('<'));
		i++;
	}
	return (1);
}

static int	check_pipes(char *line)
{
	size_t	i;
	size_t	check;
	bool	pipe;

	i = 0;
	if (line[0] == '|')
		return (print_syntax_error(line[0]));
	pipe = false;
	while (line[i] != '\0')
	{
		check = skip_quotes(&line[i]);
		if (check != 0)
		{
			i += check;
			pipe = false;
		}
		if (check_error_pipe(&line[i], &pipe) < 0)
			return (print_syntax_error('|'));
		i++;
	}
	return (1);
}

int	check_syntax(char *line)
{
	if (ft_strchr(line, '|') != 0)
	{
		if (check_pipes(line) < 0)
			return (-1);
	}
	if (ft_strchr(line, '<') != 0)
	{
		if (check_redirect_in(line) < 0)
			return (-1);
	}
	if (ft_strchr(line, '>') != 0)
	{
		if (check_redirect_out(line) < 0)
			return (-1);
	}
	if (ft_strchr(line, '\'') != 0 || ft_strchr(line, '\"') != 0)
	{
		if (check_quotes(line) < 0)
			return (-1);
	}
	return (1);
}
