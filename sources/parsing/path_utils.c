/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 13:57:01 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/11 15:09:34 by mrahmat-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./minishell.h"

void	close_all(t_files files, int pipefd[2])
{
	if (files.infile > -1)
		close(files.infile);
	if (files.outfile > -1)
		close(files.outfile);
	close(pipefd[0]);
	close(pipefd[1]);
}

void	print_error(char *message, char *filename)
{
	ft_putstr_fd(message, 2);
	ft_putstr_fd(": ", 2);
	if (filename)
		ft_putstr_fd(filename, 2);
	ft_putstr_fd("\n", 2);
	errno = 0;
}
