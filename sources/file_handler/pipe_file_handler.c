/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_file_handler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:07:28 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/10/23 17:05:19 by mrahmat-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_pipe_fd(t_list **cmd_table)
{
	int		pipe_fd[2];
	t_list	*cmd_iter;

	if (*cmd_table == NULL)
		return (1);
	cmd_iter = *cmd_table;
	while (cmd_iter->next != NULL)
	{
		if (pipe(pipe_fd) == -1)
			return (1);
		((t_files *)((t_cmd *)cmd_iter->next->content)->fd)->infile = \
			pipe_fd[0];
		((t_files *)((t_cmd *)cmd_iter->content)->fd)->outfile = pipe_fd[1];
		cmd_iter = cmd_iter->next;
	}
	return (0);
}
