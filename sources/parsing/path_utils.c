/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 13:57:01 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/06 10:43:46 by mrahmat-         ###   ########.fr       */
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
	ft_putstr_fd(filename, 2);
	ft_putstr_fd("\n", 2);
}

//TODO: does this free the null terminator
void	free_strv(char **strv)
{
	int	i;

	if (strv)
	{
		i = 0;
		while (strv[i])
		{
			free(strv[i]);
			i++;
		}
		free(strv);
	}
}
