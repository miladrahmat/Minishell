/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 15:32:24 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/14 14:03:22 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_quoted_heredoc_delim(t_redir *redir)
{
	if (is_double_quoted_str(redir->filename))
	{
		str_del_first_last(redir->filename);
		redir->heredoc_quoted_delim = true;
	}
}

int	get_redir_destroyer(t_redir *redir)
{
	if (redir && redir->filename)
		free(redir->filename);
	if (redir)
		free(redir);
	return (-1);
}
