/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_table.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 09:48:22 by lemercie          #+#    #+#             */
/*   Updated: 2024/10/07 15:39:03 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// > output
// >> output append
// < input
// << input heredoc
// TODO: leaks memory from get_word()
int	get_redir(t_cmd *cmd, char *content, int i_content)
{
	if (!content[i_content + 1])
		return (-1);
	if (content[i_content] == '>')
	{
		if (content[i_content + 1] == '>')
		{
			if (!content[i_content + 2])
			{
				return (-1);
			}
			cmd->outfile = open(get_word(&content[i_content + 2]),
					   O_WRONLY | O_CREAT | O_APPEND, 0666);
			i_content++;
		}
		else
		{
			cmd->outfile = open(get_word(&content[i_content + 1]),
					O_WRONLY | O_CREAT | O_TRUNC, 0666);
		}
		i_content++;
	}
	else if (content[i_content] == '<')
	{
		if (content[i_content + 1] == '<')
		{
			if (!content[i_content + 2])
			{
				return (-1);
			}
		//	cmd->infile = read_heredoc(
		//		skip_whitespace(&content[i_content + 2]));
			i_content++;
		}
		else
		{
			cmd->infile = open(get_word(&content[i_content + 1]),
					O_RDONLY);
		}
	i_content++;
	}
	while (content[i_content] && is_whitespace(content[i_content]))
	{
		i_content++;
	}
	while (content[i_content] && !is_whitespace(content[i_content]))
	{
		i_content++;
	}
	return (i_content);
}

// find redirects and remove them
void	parse_redirs(t_cmd *cmd, char *content)
{
	char	*cmd_no_redir;
	int		i_content;
	int		i_cmd;

	cmd_no_redir = malloc(sizeof(char) * (ft_strlen(content) + 1));
	i_content = 0;
	i_cmd = 0;
	while (content[i_content])
	{
		if (content[i_content] == '<' || content[i_content] == '>')
		{
			i_content = get_redir(cmd, content, i_content);
		}
		else
		{
			cmd_no_redir[i_cmd] = content[i_content];
			i_content++;
			i_cmd++;
		}
	}
	cmd_no_redir[i_cmd] = '\0';
	//printf("parse_redirs: %s\n", cmd_no_redir);
	//cmd->cmd = ft_split(cmd_no_redir, ' ');
	cmd->token = cmd_no_redir;
}

void	parse_token(t_cmd *cmd, char *content)
{
	parse_redirs(cmd, content);
}

// init each t_cmd of the list
void	*token_to_cmd(void *content)
{
	t_cmd	*cmd;
	
	cmd = malloc(sizeof(t_cmd));
	cmd->infile = 0;
	cmd->outfile = 1;
	cmd->path_error = 0;
	parse_token(cmd, content);
	return (cmd);
}

t_list	*init_cmd_table(t_list *tokens, char **envp)
{
	t_list	*cmd_table;
	t_list	*list_iter;
	t_cmd	*cmd;

	cmd_table = ft_lstmap(tokens, &token_to_cmd, &free);
	list_iter = cmd_table;
	while (list_iter)
	{
		cmd = (t_cmd *) list_iter->content;
		cmd->cmd = get_exec_path(cmd->token, envp, &cmd->path_error);
		list_iter = list_iter->next;
	}
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
