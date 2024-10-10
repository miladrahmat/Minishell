/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 13:31:14 by lemercie          #+#    #+#             */
/*   Updated: 2024/10/10 14:27:52 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h> // printf
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h> // bool type
# include <stdlib.h> // malloc()
# include <fcntl.h> // open()
# include <errno.h>
# include <unistd.h>
# include "libft.h"
# include "vector.h"

typedef enum e_redir_type
{
	error,
	out_trunc,
	out_append,
	input,
	heredoc
}	t_redir_type;

typedef struct s_redir
{
	t_redir_type	redir_type;
	char			*filename;
}	t_redir;

typedef struct s_files
{
	int	infile;
	int	outfile;
}	t_files;

typedef struct s_cmd
{
	char	*token;
	char	**cmd_args;
	t_list	*infiles;
	t_list	*outfiles;
	char	**envp;
	int		path_error;
}	t_cmd;

// cmd_table.c
t_list	*init_cmd_table(t_list *tokens, t_list *env);
// path_utils.c
void	open_files(t_files *files, char *infile_name, char *outfile_name);
void	close_all(t_files files, int pipefd[2]);
void	print_error(char *message, char *filename);
void	free_strv(char **strv);
// path_helpers.c
bool	is_abs_or_pwd_path(char *cmd);
int		check_exec_access(char *cmd);
char	**get_paths(t_list *env);
// paths.c
char	**get_exec_path(char *command, t_list *env, int *path_error);
// parser.c
t_list	*tokenize(char *line);

//builtin_cmds
bool	test_builtin_cmd(char *cmd);
int		check_builtin_cmd(char **cmd, int fd, t_list **envp);
int		echo(char **str, int fd);
int		env(char **cmd, int fd, t_list **envp);
int		unset(char **cmd, t_list **envp);

//helper functions
void	split_free(char **str);

// string_utils.c
bool	is_whitespace(char c);
char	*skip_whitespace(char *s);
int		substr_len(char *start, char *end);
char	*get_word(char *start);

#endif
