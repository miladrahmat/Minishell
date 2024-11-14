/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 16:02:53 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/14 14:18:24 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*increment_suffix(char	*s)
{
	static int	suffix = 0;

	s = ft_itoa(suffix);
	suffix++;
	return (s);
}

char	*create_filename(void)
{
	const char	*prefix = ".minishell-heredoc";
	char		*suffix;
	char		*filename;

	suffix = NULL;
	filename = ft_strdup(prefix);
	if (!filename)
		return (NULL);
	while (access(filename, F_OK) == 0)
	{
		free(filename);
		suffix = increment_suffix(suffix);
		if (!suffix)
			return (NULL);
		filename = ft_strjoin(prefix, suffix);
		free(suffix);
		if (!filename)
			return (NULL);
	}
	return (filename);
}

void	try_expand_write(char *line, t_env *env, int fd, bool expand)
{
	char	*expanded_line;

	if (expand)
	{
		expanded_line = expand_vars(line, env, 0);
		write(fd, expanded_line, ft_strlen(expanded_line));
		free(expanded_line);
	}
	else
		write(fd, line, ft_strlen(line));
}

int	heredoc_free_str(char *s)
{
	if (s)
		free(s);
	return (1);
}
