/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 13:57:01 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/14 18:37:12 by mrahmat-         ###   ########.fr       */
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
