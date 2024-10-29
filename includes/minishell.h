/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 13:31:14 by lemercie          #+#    #+#             */
/*   Updated: 2024/10/29 14:18:00 by mrahmat-         ###   ########.fr       */
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
# include <sys/wait.h>
# include <signal.h>
# include "libft.h"

typedef struct s_env
{
	char			*key;
	char			*value;
	bool			flag;
	struct s_env	*next;
}	t_env;

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
	bool			heredoc_quoted_delim;
}	t_redir;

typedef struct s_files
{
	int	infile;
	int	outfile;
}	t_files;

typedef struct s_cmd
{
	t_list	*split_token;
	char	**cmd_args;
	t_list	*infiles;
	t_list	*outfiles;
	int		path_error;
	t_files	*fd;
}	t_cmd;

// cmd_table.c
t_list	*init_cmd_table(char *line, t_env *env, int last_ret_val);
// path_utils.c
void	open_files(t_files *files, char *infile_name, char *outfile_name);
void	close_all(t_files files, int pipefd[2]);
void	print_error(char *message, char *filename);
void	free_strv(char **strv);
// path_helpers.c
bool	is_abs_or_pwd_path(char *cmd);
int		check_exec_access(char *cmd);
char	**get_paths(t_env *env);
// paths.c
char	*get_exec_path(char *command, t_env *env, int *path_error);
// split_on_pipes.c
t_list	*split_on_pipes(char *line);
// parsing/heredoc.c
int		process_heredocs(t_list *cmd_table, t_env *env);

//builtin_cmds
bool	test_builtin_cmd(char *cmd);
int		check_builtin_cmd(char **cmd, int fd, t_env **envp);
int		echo(char **str, int fd);
int		env(char **cmd, int fd, t_env **envp);
int		unset(char **cmd, t_env **envp);
int		export(char **cmd, int fd, t_env **envp);
int		print_builtin_error(char *cmd, char *arg, char *err, bool alloc);
t_env	*print_export_error(char **variable);
void	update_pwd(t_env **envp);
int		builtin_exit(char **cmd, t_env **envp);

//helper functions
int		split_free(char **str, int ret_val);
void	connect_list(t_env **list, t_env **node);
bool	check_key(char *cmd, t_env *node);
size_t	get_cmd_amount(char **cmd);
size_t	ft_strlen_eq(char *str);
int		validate_str(char *str, char *acc_values);

//env struct functions
t_env	*ft_envnew(char *key, char *value);
void	ft_envadd_back(t_env **lst, t_env *new);
void	ft_envadd_front(t_env **lst, t_env *new);
void	ft_envdelone(t_env *lst, void (*del)(void *));
void	ft_envclear(t_env **lst, void (*del)(void *));
void	ft_env_free_add(t_env *lst, char *key, char *value);
t_env	*set_key_value(char *str);
t_env	*ft_envcpy(t_env *envp);
int		ft_envsize(t_env *env);
char	*ft_env_get_value_by_key(char *key, t_env *env);

// string_utils.c
bool	is_whitespace(char c);
char	*skip_whitespace(char *s);
int		substr_len(char *start, char *end);
char	*get_word(char *start);
char	*skip_word(char *s);

// expand_vars.c
char	*expand_vars(char *token, t_env *env, int last_ret_val);

// file_handler.c
int		open_infiles(t_list **cmd_table);
int		open_outfiles(t_list **cmd_table);
// int		close_in_out(t_list **cmd_table);

// pipe_file_handler.c
// int		check_pipe_fd(t_list **cmd_table);
int		check_pipe_fd(t_cmd **curr_cmd, t_cmd **next_cmd);
void	close_cmd_fd(t_cmd *curr_cmd);

// tokenizing_utils.c
char	*get_token(char *start, char *end);

//execute
int		prepare_exec(t_list *cmd_table, t_env **env);
char	*copy_env_node(t_env *env);
int		execute_one_builtin(t_list *cmd_table, t_env **env);

//signals
void	handle_signals(int signal);
void	ignore_sigint(void);
void	handle_sigint(void *func);
void	signal_handling_child(void);
void	exit_signal(t_list **cmd_table, t_env **env);

//utils
void	destroy_tlist_of_tcmd(void	*arg);

#endif
