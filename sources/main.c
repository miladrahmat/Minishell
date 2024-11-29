/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:35:34 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/29 16:38:55 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// DEBUG
#include <stdio.h>
// DEBUG

sig_atomic_t	g_got_signal = 0;

void	handle_signals(int signal)
{
	if (signal == SIGINT)
	{
		g_got_signal = 130;
		ft_putchar_fd('\n', 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		return ;
	}
}

void	handle_heredoc(int signal)
{
	if (signal == SIGINT)
	{
		g_got_signal = 130;
		rl_replace_line("\n", 0);
		rl_done = 1;
		return ;
	}
}

static int	exec_cmd_table(t_list *cmd_table, t_env **env, int last_ret_val)
{
	t_list	*cmd_table_iter;
	int		ret_val;

	cmd_table_iter = cmd_table;
	while (cmd_table_iter->next != NULL)
		cmd_table_iter = cmd_table_iter->next;
	g_got_signal = 0;
	if (process_heredocs(cmd_table, *env) == 1)
		return (1);
	update__(((t_cmd *)cmd_table_iter->content)->cmd_args, env);
	if (cmd_table != NULL && g_got_signal == 0)
	{
		if (((t_cmd *)cmd_table_iter->content)->path_error == 0)
			ret_val = prepare_exec(cmd_table, env, last_ret_val);
		else
		{
			prepare_exec(cmd_table, env, last_ret_val);
			ret_val = ((t_cmd *)cmd_table_iter->content)->path_error;
		}
		check_child_signal(ret_val);
	}
	else
		ret_val = g_got_signal;
	return (ret_val);
}

static void	prepare_cmd(char *line, t_env **env, int *last_ret_val)
{
	int		check;
	t_list	*cmd_table;

	cmd_table = NULL;
	if (g_got_signal != 0)
		*last_ret_val = g_got_signal;
	if (line)
	{
		check = check_syntax(line);
		if (check > 0)
		{
			cmd_table = init_cmd_table(line, *env, *last_ret_val);
			add_history(line);
			free(line);
			if (cmd_table)
				*last_ret_val = exec_cmd_table(cmd_table, env, *last_ret_val);
		}
		else if (check == -1)
			line_has_syntax_error(line, last_ret_val);
		ft_lstclear(&cmd_table, &destroy_tlist_of_tcmd);
	}
	else
		exit_signal(&cmd_table, env, *last_ret_val);
}

int	main(int ac, char **av, char **envp)
{
	char				*line;
	t_env				*env;
	int					last_ret_val;
	int					check;

	(void)av;
	(void)ac;
	handle_sigint(&handle_signals);
	check = 0;
	env = copy_env(envp, &check);
	if (check == 1)
	{
		ft_envclear(&env, &free);
		exit(1);
	}
	last_ret_val = 0;
	while (true)
	{
		line = readline("[MINISHELL]$> ");
		prepare_cmd(line, &env, &last_ret_val);
	}
}
