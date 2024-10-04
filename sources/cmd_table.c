/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_table.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 09:48:22 by lemercie          #+#    #+#             */
/*   Updated: 2024/10/04 17:00:31 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse_token(t_cmd *cmd, char *content)
{
	char	*cmd_no_redir;
	int		i_content;
	int		i_cmd;

	cmd_no_redir = malloc(sizeof(char) * (ft_strlen(content) + 1));
	i_content = 0;
	i_cmd = 0;
	while (content[i_content])
	{
		// goto < or >
		// skip space
		// get word
		while (content[i_content] != '<' && content[i_content] != '>')
		{
			cmd_no_redir[i_cmd] = content[i_content];
			i_content++;
			i_cmd++;
		}
		
	}
		
}
// find redirects and remove them
void	*token_to_cmd(void *content)
{
	t_cmd	*cmd;
	
	cmd = malloc(sizeof(t_cmd));
	parse_token(cmd, content);
	return (cmd);
}

t_list	*init_cmd_table(t_list *tokens)
{
	t_list	*cmd_table;

	cmd_table = ft_lstmap(tokens, &token_to_cmd, &free);
	return (cmd_table);
}
/*
void	init_cmd_table(t_list *tokens)
{
	int		n_cmds;
	t_cmd	**cmd_table;
	int		i;

	n_cmds = ft_lstsize(tokens);
	cmd_table = malloc(sizeof(t_cmd *) * (n_cmds + 1));
	i = 0;
	while (i < n_cmds)
	{
		cmd_table[i] = create_cmd_table(tokens->content);
		i++;
		tokens = tokens->next;
	}
	cmd_table[n_cmds] = NULL;
}
*/
