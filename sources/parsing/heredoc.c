/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 15:38:15 by lemercie          #+#    #+#             */
/*   Updated: 2024/10/23 17:39:52 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// MULTIPLE HEREDOCS IN BASH
// have to be opened and closed in trder from left to right
// but only the input from the last one is passed forward

char	*create_filename()
{
	return ("asd");
}

void	read_into_file(int fd, char *delim)
{
	char	*line;

	line = get_next_line(fd);
	while (line)
	{
		// if line contains delim
		// write into file the part until delim
		// break
		// else
		// write into file
		free(line);
		line = get_next_line(fd);
	}
}

int	get_heredoc(char *delim)
{	//create unique filename
	// open file
	// gnl until delimiter
	// and write into the file
	// close file
	// open file for reading
	// return fd for reading

	char	*filename;
	int		write_fd;
	int		read_fd;

	filename = create_filename();
	write_fd = open(filename, O_WRONLY | O_CREAT, 0666);
	free(filename);
	read_into_file(write_fd, delim);
	close(write_fd);
	read_fd = open(filename, O_RDONLY);
	return (read_fd);
}

// to be used with ft_lstiter()
// in case of heredoc, the filename field is reused to store the delimiter
void	process_heredocs(void *arg)
{
	t_cmd	*cmd;
	t_list	*infiles_iter;
	int		fd;

	cmd = (t_cmd *) arg;
	infiles_iter = cmd->infiles;
	while (infiles_iter)
	{
		if (((t_redir *) infiles_iter->content)->redir_type == heredoc)
		{
			fd = get_heredoc(((t_redir *) infiles_iter->content)->filename);
			cmd->fd->infile = fd;
		}
		infiles_iter = infiles_iter->next;
	}

}
