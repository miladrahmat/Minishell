/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 13:24:10 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/08 09:54:55 by mrahmat-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_list(void *arg)
{
	printf("pl: %s\n", (char *) arg);
}

void	print_tlist_string(void *arg)
{
	t_redir	*redir;

	redir = (t_redir *) arg;
	printf("%s;", redir->filename);
}

// to be passed to ft_lstiter()
void	print_cmd_list(void *arg)
{
	t_cmd	*node;
	int		i;

	node = (t_cmd *) arg;
	printf("cmd_args: ");
	i = 0;
	while (node->cmd_args[i])
	{
		printf("%s;", node->cmd_args[i]);
		i++;
	}
	printf("\n");
	printf("files: ");
	ft_lstiter(node->files, &print_tlist_string);
	printf("\n");
}
