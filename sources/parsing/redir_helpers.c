/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 15:32:24 by lemercie          #+#    #+#             */
/*   Updated: 2024/12/04 10:55:24 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// at this point the delimiter is stored in redir->filename
void	check_quoted_heredoc_delim(t_redir *redir)
{
	char	*temp;
	int		trash;

	trash = 0;
	temp = strip_quotes(redir->filename, &trash);
	if (ft_strcmp(temp, redir->filename) != 0)
		redir->heredoc_quoted_delim = true;
	free(redir->filename);
	redir->filename = temp;
}

int	get_redir_destroyer(t_redir *redir)
{
	if (redir && redir->filename)
		free(redir->filename);
	if (redir)
		free(redir);
	return (-1);
}
