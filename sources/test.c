/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:35:34 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/05 15:54:25 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

sig_atomic_t	g_last_ret_val = 0;

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
//		rl_redisplay();
		rl_done = 1;
		return ;
	}
}

t_env	*copy_env(char **envp)
{
	t_env	*env;
	t_env	*new_node;

	env = NULL;
	while (*envp != NULL)
	{
		new_node = set_key_value(*envp);
		if (new_node == NULL)
		{
			ft_envclear(&env, &free);
			return (NULL);
		}
		ft_envadd_back(&env, new_node);
		envp++;
	}
	return (env);
}

void	print_tlist_string(void *arg)
{
	t_redir	*redir;

	redir = (t_redir *) arg;
	printf("%s;", redir->filename);
}

// to be passed to ft_lstiter()
void	print_cmd_list(void *arg)
{
	t_cmd	*node;
	int		i;

	node = (t_cmd *) arg;
	printf("cmd_args: ");
	i = 0;
	while (node->cmd_args[i])
	{
		printf("%s;", node->cmd_args[i]);
		i++;
	}
	printf("\n");
	printf("files: ");
	ft_lstiter(node->files, &print_tlist_string);
	printf("\n");
}

// TODO: variable names cannot start with number
int	main(int ac, char **av, char **envp)
{
	char	*line;
	t_list	*cmd_table;
	t_env	*env;

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
			cmd_table = init_cmd_table(line, env, g_last_ret_val);
			add_history(line);
			free(line);
			process_heredocs(cmd_table, env); // returns 1 in case of malloc fail
			if (cmd_table != NULL)
			{
				open_infiles(&cmd_table);
				//ft_lstiter(cmd_table, &print_cmd_list);
				g_last_ret_val = prepare_exec(cmd_table, &env);
			}
			ft_lstclear(&cmd_table, &destroy_tlist_of_tcmd);
		}
		else
			exit_signal(&cmd_table, &env);
	}
}
