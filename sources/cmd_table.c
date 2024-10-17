/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_table.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 09:48:22 by lemercie          #+#    #+#             */
/*   Updated: 2024/10/17 17:04:32 by lemercie         ###   ########.fr       */
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

// split a string on whitespace and arrows, but not in quotes
t_list	*split_token(char *cmd_token)
{
	char	*start;
	char	*end;
	t_list	*new_token;
	t_list	*tokens;
	int		quotes;

	if (!cmd_token)
		return NULL;
	start = cmd_token;
	tokens = NULL;
	end = start;
	quotes = 0;
	printf("split_token incoming token: %s\n", end);
	while (*start && *end)
	{
		start = skip_whitespace(start);
		end = start;
		if (*end == '<' || *end == '>')
			end++;
		while (*end)
		{
			if (quotes == 0)
			{
				if (*end == '\'')
					quotes = 1;
				else if (*end == '\"')
					quotes = 2;
				else if (*end == '<' || *end == '>' || is_whitespace(*end))
					break ;
			}
			else if (quotes == 1 && *end == '\'')
				quotes = 0;
			else if (quotes == 2 && *end == '\"')
				quotes = 0;
			end++;
		}
		new_token = ft_lstnew(get_token(start, end));
		ft_lstadd_back(&tokens, new_token);
			start = end;
	}
	return (tokens);
}

void	print_list(void *arg)
{
	printf("pl: %s\n", (char *) arg);
}

// init each t_cmd of the list (but not cmd->cmd_args yet)
// for each node in t_list tokens, creates a node in t_list of t_cmd
void	*init_t_cmd(void *content)
{
	t_cmd	*cmd;
	static int i = 0;
	printf("init_t_cmd %ith cmd\n", i);
	i++;
	
	cmd = malloc(sizeof(t_cmd));
	cmd->infiles = NULL;
	cmd->outfiles = NULL;
	cmd->path_error = 0;
	printf("init_t_cmd input: %s\n", (char*)content);
	cmd->split_token = split_token(content);
	ft_lstiter(cmd->split_token, &print_list);
	// now we are inside of a single t_cmd node; so loop through the tokens
//	parse_redirs(cmd, content);
	return (cmd);
}


//TODO: after splitting on pipes (taking quotes into account)
//
// env vars can expand into commands, arguments or redir filenames,
// 		but cannot contain < or > in themselves
// 	==> split on spaces and redir symbols
//
//if a token is not a redir, its a command or an argument
//	=> split further into redirs commands and arguments
//
// can return NULL in case of a failed malloc() in functions called from here
t_list	*init_cmd_table(char *line, t_env *env)
{
	t_list	*tokens;
	t_list	*cmd_table;
	t_list	*list_iter;
	t_cmd	*cmd;

	// split line at pipes into t_list of strings
	tokens = split_on_pipes(line);
//	ft_lstiter(tokens, &print_list);
	printf("init_cmd_table(): n tokens: %i\n", ft_lstsize(tokens));
	printf("=========\n");
	// TODO: tokenize further before parsing redirs and expanding variables,
	// so that quotes are not that hard every time
	//
	// convert t_list of strings into t_list of t_cmd
	cmd_table = ft_lstmap(tokens, &init_t_cmd, &free);
	list_iter = cmd_table;
	while (list_iter)
	{
		cmd = (t_cmd *) list_iter->content;
//		printf("token: %s\n", cmd->token);
	//	ft_lstiter(cmd->split_token, &print_list);
	//	cmd->token = expand_vars(cmd->token, env);
		/*
		if (!cmd->token)
			return (NULL);
		if (!test_builtin_cmd(ft_split(cmd->token, ' ')))
		{
			printf("not a builtin command, %s\n", cmd->token);
			cmd->cmd_args = get_exec_path(cmd->token, env, &cmd->path_error);
		}
		else
		{
		*/
		(void) env;
			cmd->cmd_args = NULL;
			/*
			cmd->cmd_args = malloc(sizeof(char *) * 2);
			cmd->cmd_args[0] = ft_strdup(cmd->token);
			cmd->cmd_args[1] = NULL;
			*/
	//	}
		list_iter = list_iter->next;
	}
	return (cmd_table);
}
