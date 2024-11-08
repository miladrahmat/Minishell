/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:35:34 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/08 17:58:56 by mrahmat-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

sig_atomic_t	g_last_ret_val = 0;

void	check_child_signal(int ret_val)
{
	if (ret_val == 130)
	{
		ft_putchar('\n');
		return ;
	}
	if (ret_val == 131)
	{
		ft_putendl_fd("Quit (core dumped)", 2);
		return ;
	}
}

void	handle_signals(int signal)
{
	if (signal == SIGINT)
	{
		g_last_ret_val = 130;
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
		g_last_ret_val = 130;
		rl_replace_line("", 0);
		rl_done = 1;
		return ;
	}
}

void	inner_loop(t_list *cmd_table, t_env **env)
{
	t_list	*cmd_table_iter;
	int		last_ret_val;

	cmd_table_iter = cmd_table;
	while (cmd_table_iter->next != NULL)
		cmd_table_iter = cmd_table_iter->next;
	last_ret_val = g_last_ret_val;
	g_last_ret_val = 0;
	process_heredocs(cmd_table, *env); // returns 1 in case of malloc fail
	if (cmd_table != NULL && g_last_ret_val == 0)
	{
		if (((t_cmd *)cmd_table_iter->content)->path_error == 0)
			g_last_ret_val = prepare_exec(cmd_table, env, last_ret_val);
		else
		{
			prepare_exec(cmd_table, env, last_ret_val);
			g_last_ret_val = ((t_cmd *)cmd_table_iter->content)->path_error;
		}
	}
	check_child_signal(g_last_ret_val);
}

// TODO: variable names cannot start with number
int	main(int ac, char **av, char **envp)
{
	char				*line;
	t_list				*cmd_table;
	t_env				*env;

	(void)av;
	(void)ac;
	handle_sigint(&handle_signals);
	env = copy_env(envp);
	if (env == NULL)
		exit(1);
	cmd_table = NULL;
	while (true)
	{
		line = readline("\e[1;32m[MINISHELL]$> \e[0m");
		if (line)
		{
			if (check_syntax(line) > 0)
			{
				cmd_table = init_cmd_table(line, env, g_last_ret_val);
				if (cmd_table)
					inner_loop(cmd_table, &env);
			}
			else
				g_last_ret_val = 2;
			add_history(line);
			free(line);
			ft_lstclear(&cmd_table, &destroy_tlist_of_tcmd);
		}
		else
			exit_signal(&cmd_table, &env, g_last_ret_val);
	}
}
