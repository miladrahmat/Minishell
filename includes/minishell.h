/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 13:31:14 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/29 16:12:17 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h> // bool type
# include <fcntl.h> // open()
# include <errno.h>
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

typedef struct s_list_and_index
{
	t_list	*lst;
	int		index;
	char	**env_copy;
	pid_t	*pids;
}	t_list_and_index;

typedef struct s_var_expander
{
	t_env	*env;
	int		*last_ret_val;
	bool	in_dquote;
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

// parsing/build_cmd_args.c
int			build_cmd_args(t_cmd *cmd, t_env *env);
// parsing/cmd_table.c
t_list		*init_cmd_table(char *line, t_env *env, int last_ret_val);
// parsing/expand_vars.c
char		*expand_vars(char *token, t_env *env, int *last_ret_val);
// parsing/expand_vars_helpers.c
char		*skip_varname(char *s);
char		*get_varname(char *start);
bool		is_varname(char c);
int			not_varname(char **start, char **end, char **ret, bool in_dquote);
int			expand_vars_in_filenames(t_cmd *cmd, t_env *env, int *last_ret_val);
// expand_vars_process_var.c
int			process_questionmark(char **end, char **ret, \
					t_var_expander extra_args);
int			process_env_var(char **end, char **ret, char *varname, \
					t_var_expander extra_args);
// expand_vars_utils.c
int	stupid_join(char **s1, char *s2, bool s2_alloc);
char	*concatenate_until(char **dst, char *src, char *delim);
// parsing/heredoc.c
int			process_heredocs(t_list *cmd_table, t_env *env);
// parsing/heredoc_helpers.c
char		*create_filename(void);
void		try_expand_write(char *line, t_env *env, int fd, bool expand);
int			heredoc_free_str(char *s);
// parsing/path_utils.c
void		close_all(t_files files, int pipefd[2]);
// parsing/path_helpers.c
bool		is_abs_or_pwd_path(char *cmd);
int			check_exec_access(char *cmd);
int			check_exec_access_print_err(char *cmd);
char		**get_paths(t_env *env, int *err);
bool		is_directory(char *path);
// parsing/paths.c
char		*get_exec_path(char *command, t_env *env, int *path_error);
// parsing/quotes.c
bool		is_quoted_str(char *s);
bool		is_double_quoted_str(char *s);
void		str_del_first_last(char *s);
char		*strip_quotes(char *s, int *ret_val);
int			handle_quotes(char *new, char *org, \
	size_t *new_i, ssize_t *org_i);
// parsing/redir.c
int			parse_redir_loop(t_list *cmd_table);
// parsing/redir_helpers.c
void		check_quoted_heredoc_delim(t_redir *redir);
int			get_redir_destroyer(t_redir *redir);
// parsing/redir_get_filename.c
int			get_filename_wrapper(t_redir *redir, int *tokens_consumed, \
						char *token1, char *token2);
// parsing/split_token.c
t_list		*split_token(char *cmd_token);
// parsing/split_on_pipes.c
t_list		*split_on_pipes(char *line);
// parsing/string_utils.c
char		*ft_strndup(const char *s1, size_t len);
char		*skip_until(char *s, char delim);
bool		is_whitespace(char c);
char		*skip_whitespace(char *s);
char		*ft_strjoin_safe(char const *s1, char const *s2);
// parsing/string_utils_words.c
int			substr_len(char *start, char *end);
char		*get_word(char *start);
char		*get_word_quote(char *start);
char		*skip_word(char *s);
// parsing/transform_tokens.c
int			transform_tokens1(t_list **head, t_env *env, int *last_ret_val);
int			transform_tokens2(t_list **head, int *last_ret_val);
// parsing/tokenizing_utils.c
char		*get_token(char *start, char *end);

//builtin_cmds
bool		test_builtin_cmd(char *cmd);
int			check_builtin_cmd(t_list **cmd_table, t_env **envp, int ret_val);
int			check_builtin_cmd_child(t_list_and_index child_args, t_env **envp, \
	int ret_val);
int			echo(char **str, int fd);
int			env(char **cmd, int fd, t_env **envp);
int			unset(char **cmd, t_env **envp);
int			cd(char **cmd, t_env **envp);
int			pwd(int fd);
int			export(char **cmd, int fd, t_env **envp);
bool		export_existing_key(char *cmd, t_env **envp);
int			print_builtin_error(char *cmd, char *arg, char *err, bool alloc);
t_env		*print_export_error(char **variable, bool alloc);
void		update_pwd(t_env **envp);
int			builtin_exit(t_list **cmd_table, t_env **envp, int last_ret_val);
void		check_child_signal(int ret_val);

//helper functions
int			split_free(char **str, int ret_val);
void		connect_list(t_env **list, t_env **node);
int			add_new_env(char *new_variable, t_env **envp);
size_t		get_cmd_amount(char **cmd);
size_t		ft_strlen_eq(char *str);
int			validate_str(char *str, char *acc_values);
long long	exit_error_check(char **cmd);

//env struct functions
t_env		*ft_envnew(char *key, char *value);
void		ft_envadd_back(t_env **lst, t_env *new);
void		ft_envadd_front(t_env **lst, t_env *new);
void		ft_envdelone(t_env *lst, void (*del)(void *));
void		ft_envclear(t_env **lst, void (*del)(void *));
void		ft_env_free_add(t_env *lst, char *key, char *value);
t_env		*set_key_value(char *str);
t_env		*ft_envcpy(t_env *envp);
int			ft_envsize(t_env *env);
char		*ft_env_get_value_by_key(char *key, t_env *env);
void		**update_shlvl(t_env **node);
t_env		*copy_env(char **envp, int *err);
void		update__(char **cmd, t_env **env);
// environment
t_env		*set_key_value(char *str);
// file_handler.c
int			open_files(t_list **cmd_table);
void		close_cmd_fd(t_cmd *curr_cmd);

// pipe_file_handler.c
int			check_pipe_fd(t_cmd **curr_cmd, t_cmd **next_cmd);

//execute
int			prepare_exec(t_list *cmd_table, t_env **env, int last_ret_val);
char		*copy_env_node(t_env *env);
int			execute_one_builtin(t_list *cmd_table, t_env **env, int ret_val);
int			check_child_exit_code(int status);

//signals
void		handle_signals(int signal);
void		ignore_sigint(void);
void		handle_sigint(void *func);
void		signal_handling_child(void);
void		exit_signal(t_list **cmd_table, t_env **env, int ret_val);
void		handle_heredoc(int signal);
void		heredoc_signal(void *func);

//utils
void		destroy_tlist_of_tcmd(void	*arg);
void		*free_strs(char **str1, char **str2);
int			free_strs_int(char **str1, char **str2);

//syntax.c
int			check_syntax(char *line);
size_t		skip_quotes(char *str);
int			print_syntax_error(char token);
int			check_error_redir(char *str, char redir_type, \
	int *redir_num, bool *is);
int			check_error_pipe(char *str, bool *is);
int			check_whitespace(char *str);

#endif
