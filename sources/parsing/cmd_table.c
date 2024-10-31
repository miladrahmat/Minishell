/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_table.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 09:48:22 by lemercie          #+#    #+#             */
/*   Updated: 2024/10/31 12:14:40 by lemercie         ###   ########.fr       */
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

bool	is_quoted_str(char *s)
{
	return ((s[0] == '\"' && s[ft_strlen(s) - 1] == '\"') ||
		(s[0] == '\'' && s[ft_strlen(s) - 1] == '\'')); 
}

void	str_del_first_last(char *s)
{
	int	i;
	int	len;

	len = ft_strlen(s);
	i = 0;
	while (s[i] && s[i + 1])
	{
		s[i] = s[i + 1];
		i++;
	}
	s[len - 2] = 0;
}

void	check_quoted_heredoc_delim(t_redir *redir)
{
	if (is_quoted_str(redir->filename))
	{
		str_del_first_last(redir->filename);
		redir->heredoc_quoted_delim = true;
	}
}

int	get_redir(t_cmd *cmd, char *content, bool *is_valid_redir)
{
	t_redir	*redir;
	t_list	*new_node;

	if (get_redir_type(content) == error)
		return (0);
	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (1);
	redir->heredoc_quoted_delim = false;
	redir->redir_type = get_redir_type(content);
	if (redir->redir_type == out_append)
	{
		redir->filename = get_word(content + 2);
		*is_valid_redir = true;
	}
	else if (redir->redir_type == out_trunc)
	{
		redir->filename = get_word(content + 1);
		*is_valid_redir = true;
	}
	else if (redir->redir_type == input)
	{
		redir->filename = get_word(content + 1);
		*is_valid_redir = true;
	}
	else if (redir->redir_type == heredoc)
	{
		redir->filename = get_word(content + 2);
		check_quoted_heredoc_delim(redir);
//		printf("get_redir(): heredoc delim: %s\n", redir->filename);
		*is_valid_redir = true;
	}
	new_node = ft_lstnew(redir);
	if (!new_node || !redir->filename)
		return (1);
	if (redir->redir_type == out_append || redir->redir_type == out_trunc)
		ft_lstadd_back(&cmd->outfiles, new_node);
	else
		ft_lstadd_back(&cmd->infiles, new_node);
	return (0);
}

void	parse_redirs(t_cmd *cmd)
{
	t_list	*tokens;
	t_list	*tokens_iter;
	bool	is_valid_redir;

	tokens = cmd->split_token;
	tokens_iter = tokens;
	while (tokens_iter)
	{
		is_valid_redir = false;
		get_redir(cmd, tokens_iter->content, &is_valid_redir);
		if (is_valid_redir)
			ft_lstdel_and_connect(&cmd->split_token, &tokens_iter);
		if (!tokens_iter)
			return ;
		tokens_iter = tokens_iter->next;
	}
}

// env vars can expand into commands, arguments or redir filenames,
// 		but cannot contain < or > in themselves
// 	==> split on spaces and redir symbols
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
//			printf("split_token(): %s\n", new_str);
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
	cmd->fd.infile = 0;
	cmd->fd.outfile = 1;
	return (cmd);
}

// checks first token for a cmd, then just appends the rest of the tokens
int	build_cmd_args(t_cmd *cmd, t_env *env)
{	
	int		i;
	t_list	*split_tokens_iter;

	cmd->cmd_args =
		malloc(sizeof(char *) * (ft_lstsize(cmd->split_token) + 1));
	if (!cmd->cmd_args)
	{
		//cleanup function in case of malloc fails
		return (1);
	}
	if (!cmd->split_token->content || ft_strlen(cmd->split_token->content) <= 0)
	{
		ft_putstr_fd("Error: command \"\" not found\n", 2);
		return (1);
	}
	if (!test_builtin_cmd(cmd->split_token->content))
	{
//		printf("not builtin cmd, %s\n", (char *) cmd->split_token->content);
		cmd->cmd_args[0] =
			get_exec_path(cmd->split_token->content, env, &cmd->path_error);
		if (!cmd->cmd_args[0])
		{
			//TODO: if we end up here, it can either be malloc fail or 
			// command not found, so we need to check path_error
			// and cleanup in case of malloc fails
//			printf("build_cmd_args(): cmd not found\n");
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
	return (0);
}

void	strip_quotes(char *s)
{
	if (is_quoted_str(s))
		str_del_first_last(s);
}

// Can return NULL in case of a failed malloc() in functions called from here
//
// When an incorrect variable name is given, expand_vars() will return 
// an empty string. 
//
// After parsing redirs AND exppanding variables, we can assume that 
// the first token is the cmd (?)
// TODO: stop removing single quotes from inside of double quotes
// TODO: stop removing singular $ signs inside of double quotes (expand_vars())
t_list	*init_cmd_table(char *line, t_env *env, int last_ret_val)
{
	t_list	*pipe_tokens;
	t_list	*cmd_table;
	t_list	*cmd_table_iter;
	t_cmd	*cmd;
	t_list	*split_tokens_iter;
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
		if (!cmd->split_token)
		{
			cmd_table_iter = cmd_table_iter->next;
			continue ;
		}
		split_tokens_iter = cmd->split_token;
	//	ft_lstiter(cmd->split_token, &print_list);
		while (split_tokens_iter)
		{
		//	printf("init_cmd_table: %s\n", (char *) split_tokens_iter->content);
			expanded_token = expand_vars(
				split_tokens_iter->content, env, last_ret_val);
			if (!expanded_token)
			{
				//cleanup function in case of malloc fails
				return (NULL);
			}
			// expanded_token can also be an empty string (in case of a 
			// non-existant variable), but that is currently allowed
			strip_quotes(expanded_token);
			if (split_tokens_iter->content)
				free(split_tokens_iter->content);
			split_tokens_iter->content = expanded_token;
			/*
			if (ft_strlen(expanded_token) > 0)
				split_tokens_iter->content = expanded_token;
			else
				ft_lstdel_and_connect(&cmd->split_token, &split_tokens_iter);
			*/
			split_tokens_iter = split_tokens_iter->next;
		}
	//	ft_lstiter(cmd->split_token, &print_list);
		build_cmd_args(cmd, env);
		cmd_table_iter = cmd_table_iter->next;
	}
	return (cmd_table);
}
