/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 15:53:41 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/11/08 10:26:56 by mrahmat-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	print_syntax_error(char	token, char token1)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	if (token == '\0')
		ft_putstr_fd("newline", 2);
	else if (token1 != '\0')
	{
		ft_putchar_fd(token, 2);
		ft_putchar_fd(token1, 2);
	}
	else
		ft_putchar_fd(token, 2);
	ft_putchar_fd('\'', 2);
	ft_putchar_fd('\n', 2);
	return (-1);
}

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
				return (print_syntax_error(line[i], '\0'));
		}
		if (line[i] == '\"')
		{
			i++;
			while (line[i] != '\"' && line[i] != '\0')
				i++;
			if (line[i] == '\0')
				return (print_syntax_error(line[i], '\0'));
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
	char	*special_chars;

	i = -1;
	redir_num = 0;
	redir = false;
	special_chars = "<|";
	while (line[++i] != '\0')
	{
		check = skip_quotes(&line[i]);
		if (check != 0)
		{
			i += check;
			redir = false;
		}
		if (line[i] == '>')
		{
			redir = true;
			redir_num++;
			if (redir_num > 2 && line[i - 1] == '>' && line[i - 2] != '>')
				return (print_syntax_error(line[i], line[i - 1]));
			else if (redir_num > 2 && line[i - 1] != '>')
				return (print_syntax_error(line[i + 1], '\0'));
		}
		if (redir && ft_strchr(special_chars, line[i]) != 0)
			return (print_syntax_error(line[i], '\0'));
		if (redir && line[i + 1] == '\0')
			return (print_syntax_error(line[i + 1], '\0'));
		else if (ft_isalnum(line[i + 1]) && redir)
		{
			redir_num = 0;
			redir = false;
		}
	}
	return (1);
}

static int	check_redirect_in(char *line)
{
	size_t	i;
	size_t	check;
	int		redir_num;
	bool	redir;
	char	*special_chars;

	i = -1;
	redir_num = 0;
	redir = false;
	special_chars = ">|";
	while (line[++i] != '\0')
	{
		check = skip_quotes(&line[i]);
		if (check != 0)
		{
			i += check;
			redir = false;
		}
		if (line[i] == '<')
		{
			redir = true;
			redir_num++;
			if (redir_num > 2 && line[i - 1] == '<' && redir_num % 2 != 0)
				return (print_syntax_error(line[i], line[i - 1]));
			else if (redir_num > 2 && line[i - 1] != '<')
				return (print_syntax_error(line[i + 1], '\0'));
		}
		if (redir && ft_strchr(special_chars, line[i]) != 0)
			return (print_syntax_error(line[i], '\0'));
		if (redir && line[i + 1] == '\0')
			return (print_syntax_error(line[i + 1], '\0'));
		else if (ft_isalnum(line[i]) && redir)
		{
			redir_num = 0;
			redir = false;
		}
	}
	return (1);
}

static int	check_pipes(char *line)
{
	size_t	i;
	size_t	check;
	bool	pipe;
	char	*special_chars;

	i = 0;
	if (line[i] == '|')
		return (print_syntax_error(line[i], '\0'));
	pipe = false;
	special_chars = "|";
	while (line[i] != '\0')
	{
		check = skip_quotes(&line[i]);
		if (check != 0)
		{
			i += check;
			pipe = false;
		}
		if (line[i] == '|' && !pipe)
			pipe = true;
		else if (ft_strchr(special_chars, line[i]) != 0 && pipe)
			return (print_syntax_error('|', '\0'));
		if (line[i + 1] == '\0' && pipe)
			return (print_syntax_error('|', '\0'));
		else if (ft_isprint(line[i]) && pipe)
			pipe = false;
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
