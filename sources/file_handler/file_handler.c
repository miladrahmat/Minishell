/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 12:06:58 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/10/23 16:54:14 by mrahmat-         ###   ########.fr       */
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
		if (fd_out != 1)
			close(fd_out);
		cmd_iter = cmd_iter->next;
	}
	return (-1);
}

static int	set_infile(t_list **current, t_list **cmd)
{
	t_redir	*file;
	int		fd;

	fd = 0;
	file = (t_redir *)(*current)->content;
	if (file->redir_type == input)
		fd = open(file->filename, O_RDONLY);
	if (fd == -1)
	{
		print_builtin_error(file->filename, NULL, NULL, false);
		if (((t_cmd *)(*cmd)->content)->fd->infile != 0)
			close(((t_cmd *)(*cmd)->content)->fd->infile);
		((t_cmd *)(*cmd)->content)->fd->infile = fd;
		return (-1);
	}
	if (((t_cmd *)(*cmd)->content)->fd->infile != 0)
		close(((t_cmd *)(*cmd)->content)->fd->infile);
	((t_cmd *)(*cmd)->content)->fd->infile = fd;
	return (1);
}

int	open_infiles(t_list **cmd_table)
{
	t_list	*cmd_iter;
	t_list	*fd_iter;

	cmd_iter = *cmd_table;
	while (cmd_iter != NULL)
	{
		fd_iter = ((t_cmd *)cmd_iter->content)->infiles;
		while (fd_iter != NULL)
		{
			if (set_infile(&fd_iter, &cmd_iter) == -1)
				return (-1);
			fd_iter = fd_iter->next;
		}
		cmd_iter = cmd_iter->next;
	}
	return (open_outfiles(cmd_table));
}

static int	set_outfile(t_list **current, t_list **cmd)
{
	t_redir	*file;
	int		fd;

	fd = 1;
	file = (t_redir *)(*current)->content;
	if (file->redir_type == out_append)
		fd = open(file->filename, O_WRONLY | O_CREAT | O_APPEND, 0666);
	else if (file->redir_type == out_trunc)
		fd = open(file->filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (fd == -1)
	{
		print_builtin_error(file->filename, NULL, NULL, false);
		if (((t_cmd *)(*cmd)->content)->fd->outfile != 1)
			close(((t_cmd *)(*cmd)->content)->fd->outfile);
		((t_cmd *)(*cmd)->content)->fd->outfile = fd;
		return (-1);
	}
	if (((t_cmd *)(*cmd)->content)->fd->outfile != 1)
		close(((t_cmd *)(*cmd)->content)->fd->outfile);
	((t_cmd *)(*cmd)->content)->fd->outfile = fd;
	return (1);
}

int	open_outfiles(t_list **cmd_table)
{
	t_list	*cmd_iter;
	t_list	*fd_iter;

	cmd_iter = *cmd_table;
	while (cmd_iter != NULL)
	{
		fd_iter = ((t_cmd *)cmd_iter->content)->outfiles;
		while (fd_iter != NULL)
		{
			if (set_outfile(&fd_iter, &cmd_iter) == -1)
				return (-1);
			fd_iter = fd_iter->next;
		}
		cmd_iter = cmd_iter->next;
	}
	return (0);
}
