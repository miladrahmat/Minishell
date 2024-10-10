/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_table.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 09:48:22 by lemercie          #+#    #+#             */
/*   Updated: 2024/10/10 17:32:53 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// > output
// >> output append
// < input
// << input heredoc
t_redir_type	get_redir_type(char *content)
{
	if (!content[1])
			return (error);
		if (content[0] == '>')
		{
			if (content[1] == '>')
			{
				if (!content[2])
					return (error);
				return (out_append);
			}
			else
				return (out_trunc);
		}
		else if (content[0] == '<')
		{
			if (content[1] == '<')
			{
				if (!content[2])
					return (error);
				return (heredoc);
			}
			else
				return (input);
		}
	return (error);
}

// TODO: heredoc
// Returns index to part of content that is after the redir.
int	get_redir(t_cmd *cmd, char *content, int i_content)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	redir->redir_type = get_redir_type(&content[i_content]);
	if (redir->redir_type == out_append)
	{
		redir->filename = get_word(&content[i_content + 2]);
		ft_lstadd_back(&cmd->outfiles, ft_lstnew(redir));
		i_content += 2;
	}
	else if (redir->redir_type == out_trunc)
	{
		redir->filename = get_word(&content[i_content + 1]);
		ft_lstadd_back(&cmd->outfiles, ft_lstnew(redir));
		i_content++;
	}
	else if (redir->redir_type == input)
	{
		redir->filename = get_word(&content[i_content + 1]);
		ft_lstadd_back(&cmd->infiles, ft_lstnew(redir));
		i_content++;
	}
	else if (redir->redir_type == heredoc)
	{
		printf("get_redir(): TODO: implement heredoc\n");
		i_content += 2;
	}
	else
	{
		free(redir);
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

//	For output, it has to end up in the rightmost outfile, but intermediary 
// 		files have to be created too.
// 		For input, only the rightmost file is used, but it will try to open
// 		all files. 
/*
int	get_redir(t_cmd *cmd, char *content, int i_content)
{
	t_redir_type	redir_type;
	char			*filename;

	redir_type = get_redir_type(&content[i_content]);
	if (redir_type == out_append)
	{
		ft_lstadd_back(&cmd->infiles, ft_lstnew(get_word(&content[i_content + 2])));
		filename = get_word(&content[i_content + 2]);
		cmd->outfile = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0666);
		if (cmd->outfile == -1)
			print_error("Failed to open file", filename);
		free(filename);
		i_content += 2;
	}
	else if (redir_type == out_trunc)
	{
		filename = get_word(&content[i_content + 1]);
		cmd->outfile = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (cmd->outfile == -1)
			print_error("Failed to open file", filename);
		free(filename);
		i_content++;
	}
	else if (redir_type == input)
	{
		filename = get_word(&content[i_content + 1]);
		cmd->infile = open(filename, O_RDONLY);
		if (cmd->infile == -1)
			print_error("Failed to open file", filename);
		free(filename);
		i_content++;
	}
	else if (redir_type == heredoc)
	{
		printf("get_redir(): TODO: implement heredoc\n");
		i_content += 2;
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
*/
// find redirects and remove them from the token
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
	cmd->token = cmd_no_redir;
}

// init each t_cmd of the list (but not cmd->cmd_args yet)
void	*init_t_cmd(void *content)
{
	t_cmd	*cmd;
	
	cmd = malloc(sizeof(t_cmd));
	cmd->infiles = NULL;
	cmd->outfiles = NULL;
	cmd->path_error = 0;
	parse_redirs(cmd, content);
	return (cmd);
}

t_list	*init_cmd_table(t_list *tokens, t_list *env)
{
	t_list	*cmd_table;
	t_list	*list_iter;
	t_cmd	*cmd;

	cmd_table = ft_lstmap(tokens, &init_t_cmd, &free);
	list_iter = cmd_table;
	while (list_iter)
	{
		cmd->token = expand_vars(cmd->token, env);
		cmd = (t_cmd *) list_iter->content;
		if (test_builtin_cmd(cmd->token) == false)
			cmd->cmd_args = get_exec_path(cmd->token, env, &cmd->path_error);
		else
		{
			cmd->cmd_args = NULL;
			/*
			cmd->cmd_args = malloc(sizeof(char *) * 2);
			cmd->cmd_args[0] = ft_strdup(cmd->token);
			cmd->cmd_args[1] = NULL;
			*/
		}
		list_iter = list_iter->next;
	}
	return (cmd_table);
}
