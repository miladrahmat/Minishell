/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 12:01:27 by lemercie          #+#    #+#             */
/*   Updated: 2024/10/30 14:22:54 by mrahmat-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// maybe make it so this is a fuction that can be passed to ft_lstclear()
// TODO: if there is a heredoc, delete the file

static void	clean_files(void *arg)
{
	t_redir	*file;

	file = arg;
	if (file->filename != NULL)
	{
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
	if (cmd->cmd_args != NULL)
	{
		split_free(cmd->cmd_args, 0);
		cmd->cmd_args = NULL;
	}
	ft_lstclear(&cmd->infiles, &clean_files);
	ft_lstclear(&cmd->outfiles, &clean_files);
	//ft_lstclear(&cmd->split_token, &free);
	free(cmd);
	cmd = NULL;
}

int	split_free(char **str, int ret_val)
{
	size_t	i;

	i = 0;
	while (str[i] != NULL)
	{
		free(str[i]);
		i++;
	}
	free(str[i]);
	free(str);
	return (ret_val);
}
