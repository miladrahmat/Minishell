/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 13:31:14 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/13 14:20:03 by lemercie         ###   ########.fr       */
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
# include <sys/stat.h> //stat()
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

typedef struct s_var_expander
{
	t_env	*env;
	int		*last_ret_val;
}	t_var_expander;

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
	t_list	*files;
	int		path_error;
	t_files	fd;
}	t_cmd;

// debug.c
void			print_list(void *arg);
// parsing/build_cmd_args.c
int				build_cmd_args(t_cmd *cmd, t_env *env);
// parsing/cmd_table.c
t_list			*init_cmd_table(char *line, t_env *env, int last_ret_val);
// parsing/expand_vars.c
char			*expand_vars(char *token, t_env *env, int *last_ret_val);
// parsing/expand_vars_helpers.c
char			*skip_varname(char *s);
char			*get_varname(char *start);
bool			is_varname(char c);
int				not_varname(char **start, char **end, char **ret);
// parsing/heredoc.c
int				process_heredocs(t_list *cmd_table, t_env *env);
// parsing/path_utils.c
void			close_all(t_files files, int pipefd[2]);
void			print_error(char *message, char *filename);
// parsing/path_helpers.c
bool			is_abs_or_pwd_path(char *cmd);
int				check_exec_access(char *cmd);
int				check_exec_access_print_err(char *cmd);
char			**get_paths(t_env *env, int *err);
bool			is_directory(char *path);
// parsing/paths.c
char			*get_exec_path(char *command, t_env *env, int *path_error);
// parsing/quotes.c
bool			is_quoted_str(char *s);
bool			is_double_quoted_str(char *s);
void			str_del_first_last(char *s);
char			*strip_quotes(char *s, int *ret_val);
int				handle_quotes(char *new, char *org, \
	size_t *new_i, ssize_t *org_i);
// parsing/redir.c
int				parse_redir_loop(t_list *cmd_table);
// parsing/redir_helpers.c
void	check_quoted_heredoc_delim(t_redir *redir);
int		get_filename_wrapper(
			t_redir *redir, int *tokens_consumed, char *token1, char *token2);
int		get_redir_destroyer(t_redir *redir);
// parsing/split_token.c
t_list			*split_token(char *cmd_token);
// parsing/split_on_pipes.c
t_list			*split_on_pipes(char *line);
// parsing/transform_tokens.c
int				transform_tokens1(t_list **head, t_env *env, int *last_ret_val);
int				transform_tokens2(t_list **head, int *last_ret_val);
// parsing/tokenizing_utils.c
char			*get_token(char *start, char *end);

//builtin_cmds
bool			test_builtin_cmd(char *cmd);
int				check_builtin_cmd(t_list **cmd_table, \
	t_env **envp, int ret_val);
int				check_builtin_cmd_child(t_cmd *cmd_table, \
	t_env **envp, int ret_val);
int				echo(char **str, int fd);
int				env(char **cmd, int fd, t_env **envp);
int				unset(char **cmd, t_env **envp);
int				cd(char **cmd, t_env **envp);
int				pwd(int fd);
int				export(char **cmd, int fd, t_env **envp);
int				print_builtin_error(char *cmd, char *arg, \
	char *err, bool alloc);
t_env			*print_export_error(char **variable);
void			update_pwd(t_env **envp);
int				builtin_exit(t_list **cmd_table, \
	t_env **envp, int last_ret_val);
void			check_child_signal(int ret_val);

//helper functions
int				split_free(char **str, int ret_val);
void			connect_list(t_env **list, t_env **node);
bool			check_key(char *cmd, t_env *node);
size_t			get_cmd_amount(char **cmd);
size_t			ft_strlen_eq(char *str);
int				validate_str(char *str, char *acc_values);
long long		exit_error_check(char **cmd);

//env struct functions
t_env			*ft_envnew(char *key, char *value);
void			ft_envadd_back(t_env **lst, t_env *new);
void			ft_envadd_front(t_env **lst, t_env *new);
void			ft_envdelone(t_env *lst, void (*del)(void *));
void			ft_envclear(t_env **lst, void (*del)(void *));
void			ft_env_free_add(t_env *lst, char *key, char *value);
t_env			*set_key_value(char *str);
t_env			*ft_envcpy(t_env *envp);
int				ft_envsize(t_env *env);
char			*ft_env_get_value_by_key(char *key, t_env *env);
void			**update_shlvl(t_env **node);
t_env			*copy_env(char **envp, int *err);

// string_utils.c
char			*ft_strndup(const char *s1, size_t len);
char			*skip_until(char *s, char delim);
bool			is_whitespace(char c);
char			*skip_whitespace(char *s);
int				substr_len(char *start, char *end);
char			*get_word(char *start);
char			*get_filename(char *start);
char			*get_word_quote(char *start);
char			*skip_word(char *s);
char			*ft_strjoin_safe(char const *s1, char const *s2);

// file_handler.c
int				open_files(t_list **cmd_table);
void			close_cmd_fd(t_cmd *curr_cmd);
// int		close_in_out(t_list **cmd_table);

// pipe_file_handler.c
// int		check_pipe_fd(t_list **cmd_table);
int				check_pipe_fd(t_cmd **curr_cmd, t_cmd **next_cmd);

//execute
int				prepare_exec(t_list *cmd_table, t_env **env, int last_ret_val);
char			*copy_env_node(t_env *env);
int				execute_one_builtin(t_list *cmd_table, \
	t_env **env, int ret_val);

//signals
void			handle_signals(int signal);
void			ignore_sigint(void);
void			handle_sigint(void *func);
void			signal_handling_child(void);
void			exit_signal(t_list **cmd_table, t_env **env, int ret_val);
void			handle_heredoc(int signal);
void			heredoc_signal(void *func);

//utils
void			destroy_tlist_of_tcmd(void	*arg);
void			*free_strs(char **str1, char **str2);
int				free_strs_int(char **str1, char **str2);

//syntax.c
int				check_syntax(char *line);
size_t			skip_quotes(char *str);
int				print_syntax_error(char token);
int				check_error_redir(char *str, char redir_type, \
	int *redir_num, bool *is);
int				check_error_pipe(char *str, bool *is);
int				check_whitespace(char *str);

#endif
