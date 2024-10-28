/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 15:38:15 by lemercie          #+#    #+#             */
/*   Updated: 2024/10/28 10:36:28 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// MULTIPLE HEREDOCS IN BASH
// have to be opened and closed in order from left to right
// but only the input from the last one is passed forward
//
// vars are expanded inside of heredocs
// but not if the delimiter is in quotes!


char	*increment_suffix(char	*s)
{
	char	*ret;

	if (!s)
	{
		ret = malloc(sizeof(char) * 3);
		if (!ret)
			return (NULL);
		ret[0] = '-';
		ret[1] = 'a';
		ret[2] = 0;
		return (ret);
	}
	s[1]++;
	if (s[1] > 'z')
		printf("increment_suffix(): too many heredocs\n");
	return (s);
}

char	*create_filename()
{
	const char	*prefix = ".minishell-heredoc";
	char		*suffix;
	char		*filename;

	suffix = NULL;
	filename = ft_strdup(prefix);
	if (!filename)
		return (NULL);
	while (access(filename, F_OK) == 0)
	{
		free(filename);
		suffix = increment_suffix(suffix);
		if (!suffix)
			return (NULL);
		filename = ft_strjoin(prefix, suffix);
		if (!filename)
		{
			if (suffix)
				free(suffix);
			return (NULL);
		}
	}
//	printf("create_filename(): %s\n", filename);
	return (filename);
}

void	read_into_file(int fd, char *delim, t_env *env)
{
	char	*line;
	char	*expanded_line;

	line = readline(">");
//	printf("read_into_file(): delim: %sX\n", delim);
	while (line && ft_strcmp(line, delim) != 0)
	{
		expanded_line = expand_vars(line, env);
		write(fd, expanded_line, ft_strlen(expanded_line));
		write(fd, "\n", 1);
		free(expanded_line);
		free(line);
		line = readline(">");
	}
//	printf("leaving read_into_file()\n");
}

char	*get_heredoc(char *delim, t_env *env)
{	//create unique filename
	// open file
	// readline until delimiter
	// and write into the file
	// close file
	// then
	// return filename
	char	*filename;
	int		write_fd;

	filename = create_filename();
	if (!filename)
		return (NULL);
	write_fd = open(filename, O_WRONLY | O_APPEND | O_CREAT, 0666);
	read_into_file(write_fd, delim, env);
	close(write_fd);
	return (filename);
}

// in case of heredoc, the filename field initially used to store the delimiter
// and after processing stores the filename to the temporary file
// returns 1 in case of malloc fail
int	process_heredocs(t_list *cmd_table, t_env *env)
{
	t_cmd	*cmd;
	t_list	*infiles_iter;
	t_list	*cmd_table_iter;
	char	*filename;
	t_redir	*redir;

	(void) env;
	cmd_table_iter = cmd_table;
	while (cmd_table_iter)
	{
		cmd = (t_cmd *) cmd_table_iter->content;
		infiles_iter = cmd->infiles;
		while (infiles_iter)
		{
			redir = (t_redir *) infiles_iter->content;
			if (redir->redir_type == heredoc)
			{
				filename = get_heredoc(redir->filename, env);
				if (filename)
					redir->filename = filename;
				else
					return (1);
			}
			infiles_iter = infiles_iter->next;
		}
		cmd_table_iter = cmd_table_iter->next;
	}
	return (0);
}
