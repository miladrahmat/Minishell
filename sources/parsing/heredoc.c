/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 15:38:15 by lemercie          #+#    #+#             */
/*   Updated: 2024/10/25 12:36:11 by lemercie         ###   ########.fr       */
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
//	filename = ft_strjoin(prefix, suffix);
	filename = (char *) prefix;
	while (access(filename, F_OK) == 0)
	{
	//	free(filename);
		suffix = increment_suffix(suffix);
		filename = ft_strjoin(prefix, suffix);
	}
//	printf("create_filename(): %s\n", filename);
	return (filename);
}

// TODO: use readline
void	read_into_file(int fd, char *delim)
{
	char	*line;

	line = readline(">");
//	printf("read_into_file(): delim: %sX\n", delim);
	while (line && ft_strcmp(line, delim) != 0)
	{
		// expand vars here
		write(fd, line, ft_strlen(line));
		free(line);
		line = readline(">");
	}
//	printf("leaving read_into_file()\n");
}

// TODO: put into history
//int	get_heredoc(char *delim)
char	*get_heredoc(char *delim)
{	//create unique filename
	// open file
	// readline until delimiter
	// and write into the file
	// close file
	// then
	// return filename
	// OR
	// open file for reading
	// return fd for reading

	char	*filename;
	int		write_fd;
//	int		read_fd;

	filename = create_filename();
	write_fd = open(filename, O_WRONLY | O_APPEND | O_CREAT, 0666);
//	printf("get_heredoc(): write_fd: %i\n", write_fd);
//	free(filename);
	read_into_file(write_fd, delim);
	close(write_fd);
//	read_fd = open(filename, O_RDONLY);
//	return (read_fd);
	return (filename);
}

// in case of heredoc, the filename field initially used to store the delimiter
// and after processing stores the filename to the temporary file
void	process_heredocs(t_list *cmd_table, t_env *env)
{
	t_cmd	*cmd;
	t_list	*infiles_iter;
	t_list	*cmd_table_iter;

	(void) env;
	cmd_table_iter = cmd_table;
	while (cmd_table_iter)
	{
		cmd = (t_cmd *) cmd_table_iter->content;
		infiles_iter = cmd->infiles;
		while (infiles_iter)
		{
			if (((t_redir *) infiles_iter->content)->redir_type == heredoc)
			{
				((t_redir *) infiles_iter->content)->filename = 
					get_heredoc(((t_redir *) infiles_iter->content)->filename);
			}
			infiles_iter = infiles_iter->next;
		}
		cmd_table_iter = cmd_table_iter->next;
	}
}