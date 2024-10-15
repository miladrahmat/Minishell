/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 12:06:58 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/10/15 16:57:07 by mrahmat-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	close_in_out(t_list **cmd_table)
{
	t_list	*cmd_iter;
	int		fd_in;
	int		fd_out;

	cmd_iter = *cmd_table;
	while (cmd_iter != NULL)
	{
		fd_in = ((t_cmd *)cmd_iter->content)->fd->infile;
		if (fd_in != 0)
			close(fd_in);
		fd_out = ((t_cmd *)cmd_iter->content)->fd->outfile;
		if (fd_out != 0)
			close(fd_out);
		cmd_iter = cmd_iter->next;
	}
	return (-1);
}

int	open_infiles(t_list **cmd_table)
{
	int		fd;
	t_list	*cmd_iter;
	t_list	*fd_iter;
	t_redir	*file;

	cmd_iter = *cmd_table;
	while (cmd_iter != NULL)
	{
		fd_iter = ((t_cmd *)cmd_iter->content)->infiles;
		while (fd_iter != NULL)
		{
			file = (t_redir *)fd_iter->content;
			if (file->redir_type == input)
				fd = open(file->filename, O_RDONLY);
			if (fd == -1)
				return (close_in_out(cmd_table));
			if (((t_cmd *)cmd_iter->content)->fd->infile != 0)
				close(((t_cmd *)cmd_iter->content)->fd->infile);
			((t_cmd *)cmd_iter->content)->fd->infile = fd;
			fd_iter = fd_iter->next;
		}
		cmd_iter = cmd_iter->next;
	}
	return (open_outfiles(cmd_table));
}

int	open_outfiles(t_list **cmd_table)
{
	int		fd;
	t_list	*cmd_iter;
	t_list	*fd_iter;
	t_redir	*file;

	cmd_iter = *cmd_table;
	while (cmd_iter != NULL)
	{
		fd_iter = ((t_cmd *)cmd_iter->content)->outfiles;
		while (fd_iter != NULL)
		{
			file = (t_redir *)fd_iter->content;
			if (file->redir_type == out_append)
				fd = open(file->filename, O_WRONLY | O_CREAT | O_APPEND, 0666);
			else if (file->redir_type == out_trunc)
				fd = open(file->filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
			if (fd == -1)
				return (close_in_out(cmd_table));
			if (((t_cmd *)cmd_iter->content)->fd->outfile != 1)
				close(((t_cmd *)cmd_iter->content)->fd->outfile);
			((t_cmd *)cmd_iter->content)->fd->outfile = fd;
			fd_iter = fd_iter->next;
		}
		cmd_iter = cmd_iter->next;
	}
	return (0);
}
