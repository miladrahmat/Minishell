/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 12:01:27 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/06 11:32:33 by mrahmat-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	clean_files(void *arg)
{
	t_redir	*file;

	file = arg;
	if (file == NULL)
		return ;
	if (file->filename != NULL)
	{
		if (file->redir_type == heredoc)
			unlink(file->filename);
		free(file->filename);
		file->filename = NULL;
	}
	free(file);
	file = NULL;
}

void	destroy_tlist_of_tcmd(void	*arg)
{
	t_cmd	*cmd;

	if (arg == NULL)
		return ;
	cmd = arg;
	if (cmd == NULL)
		return ;
	if (cmd->cmd_args != NULL)
	{
		split_free(cmd->cmd_args, 0);
		cmd->cmd_args = NULL;
	}
	ft_lstclear(&cmd->files, &clean_files);
	free(cmd);
	cmd = NULL;
}

int	split_free(char **str, int ret_val)
{
	size_t	i;

	i = 0;
	if (str == NULL)
		return (ret_val);
	while (str[i] != NULL)
	{
		free(str[i]);
		i++;
	}
//	free(str[i]);
	free(str);
	return (ret_val);
}
