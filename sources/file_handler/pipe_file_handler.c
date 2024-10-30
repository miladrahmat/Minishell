/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_file_handler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:07:28 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/10/30 14:19:46 by mrahmat-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_pipe_fd(t_cmd **curr_cmd, t_cmd **next_cmd)
{
	int	pipe_fd[2];

	if (pipe(pipe_fd) == -1)
		return (-1);
	if ((*next_cmd)->fd.infile == 0)
		(*next_cmd)->fd.infile = pipe_fd[0];
	else
		close(pipe_fd[0]);
	if ((*curr_cmd)->fd.outfile == 1)
		(*curr_cmd)->fd.outfile = pipe_fd[1];
	else
		close(pipe_fd[1]);
	return (0);
}
