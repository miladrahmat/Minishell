/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 13:31:14 by lemercie          #+#    #+#             */
/*   Updated: 2024/10/10 17:55:20 by mrahmat-         ###   ########.fr       */
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

typedef struct s_env
{
	char			*key;
	char			*value;
	bool			flag;
	struct s_env	*next;
}	t_env;

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
int		check_builtin_cmd(char **cmd, int fd, t_env **envp);
int		echo(char **str, int fd);
int		env(char **cmd, int fd, t_env **envp);
int		unset(char **cmd, t_env **envp);
int		export(char **cmd, int fd, t_env **envp);

//helper functions
void	split_free(char **str);
void	connect_list(t_env **list, t_env **node);
bool	check_key(char *cmd, t_env *node);
size_t	get_cmd_amount(char **cmd);
size_t	ft_strlen_eq(char *str);

//env struct functions
t_env	*ft_envnew(char *key, char *value);
void	ft_envadd_back(t_env **lst, t_env *new);
void	ft_envadd_front(t_env **lst, t_env *new);
void	ft_envdelone(t_env *lst, void (*del)(void *));
void	ft_envclear(t_env **lst, void (*del)(void *));
void	ft_env_free_add(t_env *lst, char *key, char *value);
t_env	*get_key_value(char *str);
t_env	*ft_envcpy(t_env *envp);

#endif
