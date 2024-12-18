/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 12:06:58 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/11/06 10:40:01 by mrahmat-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_cmd_fd(t_cmd *curr_cmd)
{
	if (curr_cmd->fd.infile > 0)
		close(curr_cmd->fd.infile);
	if (curr_cmd->fd.outfile > 1)
		close(curr_cmd->fd.outfile);
}

static int	set_infile(t_list **current, t_list **cmd)
{
	t_redir	*file;
	int		fd;

	fd = 0;
	file = (t_redir *)(*current)->content;
	if (file->redir_type == heredoc || file->redir_type == input)
		fd = open(file->filename, O_RDONLY);
	if (fd == -1)
	{
		print_builtin_error(file->filename, NULL, NULL, false);
		if (((t_cmd *)(*cmd)->content)->fd.infile != 0)
			close(((t_cmd *)(*cmd)->content)->fd.infile);
		((t_cmd *)(*cmd)->content)->fd.infile = fd;
		return (-1);
	}
	if (((t_cmd *)(*cmd)->content)->fd.infile != 0)
		close(((t_cmd *)(*cmd)->content)->fd.infile);
	((t_cmd *)(*cmd)->content)->fd.infile = fd;
	return (1);
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
		if (((t_cmd *)(*cmd)->content)->fd.outfile != 1)
			close(((t_cmd *)(*cmd)->content)->fd.outfile);
		((t_cmd *)(*cmd)->content)->fd.outfile = fd;
		return (-1);
	}
	if (((t_cmd *)(*cmd)->content)->fd.outfile != 1)
		close(((t_cmd *)(*cmd)->content)->fd.outfile);
	((t_cmd *)(*cmd)->content)->fd.outfile = fd;
	return (1);
}

int	open_files(t_list **cmd_table)
{
	t_list	*fd_iter;

	fd_iter = ((t_cmd *)(*cmd_table)->content)->files;
	if (fd_iter == NULL || fd_iter->content == NULL)
		return (1);
	while (fd_iter != NULL)
	{
		if (((t_redir *)fd_iter->content)->redir_type == input \
			|| ((t_redir *)fd_iter->content)->redir_type == heredoc)
		{
			if (set_infile(&fd_iter, cmd_table) == -1)
				return (-1);
		}
		else
		{
			if (set_outfile(&fd_iter, cmd_table) == -1)
				return (-1);
		}
		fd_iter = fd_iter->next;
	}
	return (1);
}
