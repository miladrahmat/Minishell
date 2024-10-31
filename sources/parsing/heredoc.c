/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 15:38:15 by lemercie          #+#    #+#             */
/*   Updated: 2024/10/31 17:26:20 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// MULTIPLE HEREDOCS IN BASH
// have to be opened and closed in order from left to right
// but only the input from the last one is passed forward
//
// vars are expanded inside of heredocs
// but not if the delimiter is in quotes!

int	event(void)
{
	return (0);
}

char	*increment_suffix(char	*s)
{
	static int	suffix = 0;

	s = ft_itoa(suffix);
	suffix++;
	return (s);
}

char	*create_filename()
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
		if (!filename)
		{
			if (suffix)
				free(suffix);
			return (NULL);
		}
	}
//	printf("create_filename(): %s\n", filename);
	return (filename);
}

int	read_into_file(int fd, char *delim, t_env *env, bool expand)
{
	char	*line;
	char	*expanded_line;

	rl_done = 0;
	rl_event_hook = event;
	line = readline(">");
	if (!*line)
		return (1);
//	printf("read_into_file(): delim: %sX\n", delim);
	while (line && ft_strcmp(line, delim) != 0)
	{
		if (expand)
		{
			expanded_line = expand_vars(line, env, 0);
			write(fd, expanded_line, ft_strlen(expanded_line));
			free(expanded_line);
		}
		else
			write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
		line = readline(">");
		if (!*line)
			return (1);
	}
	return (0);
//	printf("leaving read_into_file()\n");
}

char	*get_heredoc(char *delim, t_env *env, bool expand)
{	//create unique filename
	// open file
	// readline until delimiter
	// and write into the file
	// close file
	// then
	// return filename
	char	*filename;
	int		write_fd;


//	printf("get_heredoc()\n");
	filename = create_filename();
	if (!filename)
		return (NULL);
	write_fd = open(filename, O_WRONLY | O_APPEND | O_CREAT, 0666);
//	write_fd = open("./", O_TMPFILE | O_WRONLY, 0666);
	heredoc_signal(&handle_heredoc);
	if (read_into_file(write_fd, delim, env, expand) == 1)
	{
//		printf("get_heredoc(): returning null: %s\n", filename);
		unlink(filename);
		return (NULL);
	}
	handle_sigint(&handle_signals);
	close(write_fd);
	return (filename);
}

// in case of heredoc, the filename field initially used to store the delimiter
// and after processing stores the filename to the temporary file
// returns 1 in case of malloc fail
int	process_heredocs(t_list *cmd_table, t_env *env)
{
	t_cmd	*cmd;
	t_list	*infiles_iter;
	t_list	*cmd_table_iter;
	char	*filename;
	t_redir	*redir;

	(void) env;
	cmd_table_iter = cmd_table;
	while (cmd_table_iter)
	{
		cmd = (t_cmd *) cmd_table_iter->content;
		infiles_iter = cmd->infiles;
		while (infiles_iter)
		{
			redir = (t_redir *) infiles_iter->content;
			if (redir->redir_type == heredoc)
			{
				if (redir->heredoc_quoted_delim)
					filename = get_heredoc(redir->filename, env, true);
				else
					filename = get_heredoc(redir->filename, env, false);
				if (filename)
					redir->filename = filename;
				else
				{
					ft_lstdel_and_connect(&cmd->infiles, &infiles_iter);
		//			redir->filename = NULL;
		//			return (1);
				}
			}
			if (infiles_iter)
				infiles_iter = infiles_iter->next;
			else
				break ;
		}
		cmd_table_iter = cmd_table_iter->next;
	}
	return (0);
}
