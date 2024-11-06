/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_table.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 09:48:22 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/06 15:53:53 by mrahmat-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*skip_until_last(char *s, char delim)
{
	char	*last;

	if (!s)
		return (NULL);
	while (*s)
	{
		if (*s == delim)
			last = s;
		if (last[1] == ' ')
			return (last);
		s++;
	}
	return (last);
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
				{
					quotes = 1;
				}
				else if (*end == '\"')
				{
					quotes = 2;
				}
				else if (*end == '<' || *end == '>' || is_whitespace(*end))
					break ;
			}
			else if (quotes == 1 && *end == '\'')
			{
				quotes = 0;
			}
			else if (quotes == 2 && *end == '\"')
			{
				quotes = 0;
			}
			end++;
		}
		new_str = get_token(start, end);
		if (new_str)
		{
	//		printf("split_token(): %s\n", new_str);
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
	cmd->cmd_args = NULL;
	cmd->files = NULL;
	cmd->path_error = 0;
//	printf("init_t_cmd input: %s\n", (char*)content);
	cmd->split_token = split_token(content);
//	ft_lstiter(cmd->split_token, &print_list);
	// now we are inside of a single t_cmd node; so loop through the tokens
	//parse_redirs(cmd);
//	ft_lstiter(cmd->split_token, &print_list);
	cmd->fd.infile = 0;
	cmd->fd.outfile = 1;
	return (cmd);
}

void	parse_redir_loop(void *arg)
{
	parse_redirs((t_cmd *) arg);
}

void	*free_strs(char **str1, char **str2)
{
	if (*str1 != NULL)
		free(*str1);
	if (*str2 != NULL)
		free(*str2);
	return (NULL);
}

// TODO: strip all quotes that are not inside of other quotes
char	*strip_quotes(char **s, int *ret_val)
{
	char	*ret;
	size_t	ret_i;
	ssize_t	s_i;
	ssize_t	add;

	ret_i = 0;
	s_i = -1;
	ret = malloc((ft_strlen(*s) + 1) * sizeof(char));
	if (ret == NULL)
		return (free_strs(s, NULL));
	while ((*s)[++s_i] != '\0')
	{
		if ((*s)[s_i] == '\'' || (*s)[s_i] == '\"')
		{
			s_i++;
			add = handle_quotes(ret + ret_i, *s + s_i, (*s)[s_i - 1]);
			if (add == -1)
			{
				*ret_val = 2;
				return (free_strs(s, &ret));
			}
			ret_i += add;
			s_i += add;
		}
		else
			ret[ret_i++] = (*s)[s_i];
	}
	free(*s);
	ret[ret_i] = '\0';
	return (ret);
}

// Can return NULL in case of a failed malloc() in functions called from here
//
// When an incorrect variable name is given, expand_vars() will return 
// an empty string. 
//
// After parsing redirs AND exppanding variables, we can assume that 
// the first token is the cmd (?)
t_list	*init_cmd_table(char *line, t_env *env, int last_ret_val)
{
	t_list	*pipe_tokens;
	t_list	*cmd_table;
	t_list	*cmd_table_iter;
	t_cmd	*cmd;
	t_list	*split_tokens_iter;
	char	*expanded_token;

	pipe_tokens = split_on_pipes(line);
//	printf("init_cmd_table(): %i tokens\n", ft_lstsize(tokens));
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
		//	printf("expanded_token: %s\n", expanded_token);
			expanded_token = strip_quotes(&expanded_token, &last_ret_val);
		//	printf("after stripping quotes: %sX\n", expanded_token);
			if (ft_strlen(expanded_token) > 0)
			{
				if (split_tokens_iter->content)
					free(split_tokens_iter->content);
				split_tokens_iter->content = expanded_token;
			}
			else
				ft_lstdel_and_connect(&cmd->split_token, &split_tokens_iter);
			
			split_tokens_iter = split_tokens_iter->next;
		}
	//	ft_lstiter(cmd->split_token, &print_list);
		cmd_table_iter = cmd_table_iter->next;
	}
	ft_lstiter(cmd_table, parse_redir_loop);
	cmd_table_iter = cmd_table;
	while (cmd_table_iter)
	{
		cmd = (t_cmd *) cmd_table_iter->content;
		if (!cmd->split_token)
		{
			cmd_table_iter = cmd_table_iter->next;
			continue ;
		}
	//	ft_lstiter(cmd->split_token, &print_list);
		build_cmd_args(cmd, env);
		cmd_table_iter = cmd_table_iter->next;
	}
	return (cmd_table);
}
