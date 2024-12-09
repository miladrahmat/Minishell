/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 15:38:15 by lemercie          #+#    #+#             */
/*   Updated: 2024/12/09 16:47:52 by mrahmat-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// MULTIPLE HEREDOCS IN BASH
// have to be opened and closed in order from left to right
// but only the input from the last one is passed forward
//
// vars are expanded inside of heredocs
// but not if the delimiter is in quotes!

static int	event(void)
{
	return (0);
}

// returns 0 on ctrl-D and 1 on ctrl-C
// 0 also on malloc fail in readline
static int	read_into_file(int fd, char *delim, t_env *env, bool expand)
{
	char	*line;

	rl_done = 0;
	rl_event_hook = event;
	line = readline("> ");
	if (!line)
		return (0);
	if (*line == '\n')
		return (heredoc_free_str(fd, line));
	while (line && ft_strcmp(line, delim) != 0)
	{
		if (*line == '\n')
			return (heredoc_free_str(fd, line));
		if (try_expand_write(line, env, fd, expand) > 0)
		{
			free(line);
			return (1);
		}
		write(fd, "\n", 1);
		free(line);
		line = readline("> ");
	}
	if (line)
		free(line);
	return (0);
}

//create unique filename
// open file
// readline until delimiter
// and write into the file
// close file
// then
// return filename

static char	*get_heredoc(char *delim, t_env *env, bool expand, int *err)
{
	char	*filename;
	int		write_fd;

	filename = create_filename();
	if (!filename)
	{
		*err = 1;
		return (NULL);
	}
	write_fd = open(filename, O_WRONLY | O_APPEND | O_CREAT, 0666);
	heredoc_signal(&handle_heredoc);
	if (read_into_file(write_fd, delim, env, expand) == 1)
	{
		*err = 1;
		unlink(filename);
		free(filename);
		return (NULL);
	}
	handle_sigint(&handle_signals);
	close(write_fd);
	return (filename);
}

// return 2 on malloc fail, 1 on signal
static int	get_heredoc_wrapper(t_redir *redir, t_env *env)
{
	char	*filename;
	int		err;

	err = 0;
	if (redir->heredoc_quoted_delim)
		filename = get_heredoc(redir->filename, env, false, &err);
	else
		filename = get_heredoc(redir->filename, env, true, &err);
	free(redir->filename);
	redir->filename = NULL;
	if (filename)
		redir->filename = filename;
	else
	{
		if (err == 1)
			return (2);
		return (1);
	}
	return (0);
}

// in case of heredoc, the filename field initially used to store the delimiter
// and after processing stores the filename to the temporary file
// returns 1 in case of malloc fail
int	process_heredocs(t_list *cmd_table, t_env *env)
{
	t_list	*files_iter;
	t_list	*cmd_table_iter;
	t_redir	*redir;

	(void) env;
	cmd_table_iter = cmd_table;
	while (cmd_table_iter)
	{
		files_iter = ((t_cmd *) cmd_table_iter->content)->files;
		while (files_iter)
		{
			redir = (t_redir *) files_iter->content;
			if (redir->redir_type == heredoc)
			{
				if (get_heredoc_wrapper(redir, env) == 2)
					return (1);
			}
			if (!files_iter)
				break ;
			files_iter = files_iter->next;
		}
		cmd_table_iter = cmd_table_iter->next;
	}
	return (0);
}
