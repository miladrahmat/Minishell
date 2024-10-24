/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_table.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 09:48:22 by lemercie          #+#    #+#             */
/*   Updated: 2024/10/23 14:38:18 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// > output
// >> output append
// < input
// << input heredoc
t_redir_type	get_redir_type(char *content)
{
	if (!content || !*content)
		return (error);
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

bool	get_redir(t_cmd *cmd, char *content)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	redir->redir_type = get_redir_type(content);
	if (redir->redir_type == out_append)
	{
		redir->filename = get_word(content + 2);
		ft_lstadd_back(&cmd->outfiles, ft_lstnew(redir));
		return (true);
	}
	else if (redir->redir_type == out_trunc)
	{
		redir->filename = get_word(content + 1);
		ft_lstadd_back(&cmd->outfiles, ft_lstnew(redir));
			return (true);
	}
	else if (redir->redir_type == input)
	{
		redir->filename = get_word(content + 1);
		ft_lstadd_back(&cmd->infiles, ft_lstnew(redir));
		return (true);
	}
	else if (redir->redir_type == heredoc)
	{
		printf("get_redir(): TODO: implement heredoc\n");
		return (true);
	}
	else
	{
		return (false);
	}
}

void	parse_redirs(t_cmd *cmd)
{
	t_list	*tokens;
	t_list	*tokens_iter;
	bool	valid_redir;

	tokens = cmd->split_token;
	tokens_iter = tokens;
	while (tokens_iter)
	{
		valid_redir = get_redir(cmd, tokens_iter->content);
		if (valid_redir)
			ft_lstdel_and_connect(&cmd->split_token, &tokens_iter);
		tokens_iter = tokens_iter->next;
	}
}

// split a string on whitespace and arrows, but not in quotes
t_list	*split_token(char *cmd_token)
{
	char	*start;
	char	*end;
	t_list	*new_token;
	t_list	*tokens;
	int		quotes;
	char	*new_str;

	if (!cmd_token)
		return NULL;
	start = cmd_token;
	tokens = NULL;
	end = start;
	quotes = 0;
//	printf("split_token incoming token: %s\n", end);
	while (*start && *end)
	{
		start = skip_whitespace(start);
		end = start;
		if (*end == '<' || *end == '>')
		{
			end++;
			if (*end == '<' || *end == '>')
				end++;
		}
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
		new_str = get_token(start, end);
		if (new_str)
		{
			new_token = ft_lstnew(new_str);
			ft_lstadd_back(&tokens, new_token);
		}
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
//	static int i = 0;
//	printf("init_t_cmd %ith cmd\n", i);
//	i++;
	
	cmd = malloc(sizeof(t_cmd));
	cmd->infiles = NULL;
	cmd->outfiles = NULL;
	cmd->path_error = 0;
//	printf("init_t_cmd input: %s\n", (char*)content);
	cmd->split_token = split_token(content);
//	ft_lstiter(cmd->split_token, &print_list);
	// now we are inside of a single t_cmd node; so loop through the tokens
	parse_redirs(cmd);
	cmd->fd = malloc(sizeof(t_files));
	cmd->fd->infile = 0;
	cmd->fd->outfile = 1;
	return (cmd);
}

// env vars can expand into commands, arguments or redir filenames,
// 		but cannot contain < or > in themselves
// 	==> split on spaces and redir symbols
//
//if a token is not a redir, its a command or an argument
//	=> split further into redirs commands and arguments
//
// can return NULL in case of a failed malloc() in functions called from here
// TODO: when an incorrect variable name is given, expand_vars() will return 
// an empty string. Do we then need to remove the token from the list?
t_list	*init_cmd_table(char *line, t_env *env)
{
	t_list	*pipe_tokens;
	t_list	*cmd_table;
	t_list	*cmd_table_iter;
	t_cmd	*cmd;
	t_list	*split_tokens_iter;
	int		i;
	char	*expanded_token;

	// split line at pipes into t_list of strings
	pipe_tokens = split_on_pipes(line);
//	printf("init_cmd_table(): %i tokens\n", ft_lstsize(tokens));
	// convert t_list of strings into t_list of t_cmd
	cmd_table = ft_lstmap(pipe_tokens, &init_t_cmd, &free);
	cmd_table_iter = cmd_table;
	while (cmd_table_iter)
	{
		cmd = (t_cmd *) cmd_table_iter->content;
		split_tokens_iter = cmd->split_token;
		while (split_tokens_iter)
		{
			expanded_token = expand_vars(split_tokens_iter->content, env);
			if (!expanded_token)
			{
				//cleanup function in case of malloc fails
				return (NULL);
			}
			// expanded_token can also be an empty string (in case of a 
			// non-existant variable), but that is currently allowed
			split_tokens_iter->content = expanded_token;
			/*
			if (ft_strlen(expanded_token) > 0)
				split_tokens_iter->content = expanded_token;
			else
				ft_lstdel_and_connect(&cmd->split_token, &split_tokens_iter);
			*/
			split_tokens_iter = split_tokens_iter->next;
		}
		cmd->cmd_args =
			malloc(sizeof(char *) * (ft_lstsize(cmd->split_token) + 1));
		if (!cmd->cmd_args)
		{
			//cleanup function in case of malloc fails
			return (NULL);
		}
		if (!test_builtin_cmd(cmd->split_token->content))
		{
			printf("not builtin cmd, %s\n", (char *) cmd->split_token->content);
			cmd->cmd_args[0] =
				get_exec_path(cmd->split_token->content, env, &cmd->path_error);
			if (!cmd->cmd_args[0])
			{
				//cleanup function in case of malloc fails
				return (NULL);
			}
		}
		else
			cmd->cmd_args[0] = cmd->split_token->content;
		i = 1;
		split_tokens_iter = cmd->split_token;
		split_tokens_iter = split_tokens_iter->next;
		while (split_tokens_iter)
		{
			cmd->cmd_args[i] = ft_strdup(split_tokens_iter->content);
			split_tokens_iter = split_tokens_iter->next;
			i++;
		}
		cmd->cmd_args[i] = NULL;
		cmd_table_iter = cmd_table_iter->next;
	}
	return (cmd_table);
}
