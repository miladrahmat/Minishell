/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 12:01:27 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/11 14:50:40 by mrahmat-         ###   ########.fr       */
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
	ft_lstclear(&cmd->split_token, &free);
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
	free(str);
	return (ret_val);
}

void	*free_strs(char **str1, char **str2)
{
	if (*str1 != NULL)
		free(*str1);
	if (*str2 != NULL)
		free(*str2);
	return (NULL);
}
