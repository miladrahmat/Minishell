/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 13:31:14 by lemercie          #+#    #+#             */
/*   Updated: 2024/10/03 15:59:46 by mrahmat-         ###   ########.fr       */
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

typedef struct s_files
{
	int	infile;
	int	outfile;
}	t_files;

// path_utils.c
void	open_files(t_files *files, char *infile_name, char *outfile_name);
void	close_all(t_files files, int pipefd[2]);
void	print_error(char *message, char *filename);
void	free_strv(char **strv);
// path_helpers.c
bool	is_abs_or_pwd_path(char *cmd);
int		check_exec_access(char *cmd);
char	**get_paths(char **envp);
// paths.c
char	**get_exec_path(char *command, char **envp, int *path_error);

//builtin_cmds
int		check_builtin_cmd(char **cmd, int fd, t_list **envp);
int		echo(char **str, int fd);
int		env(char **cmd, int fd, t_list **envp);
int		unset(char **cmd, t_list **envp);

//helper functions
void	split_free(char **str);

#endif
