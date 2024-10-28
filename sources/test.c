/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:35:34 by lemercie          #+#    #+#             */
/*   Updated: 2024/10/28 14:25:08 by mrahmat-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_last_ret_val = 0;

void	handle_sigint(void *func)
{
	struct sigaction	sigint;
	struct sigaction	sigquit;

	sigint.sa_handler = func;
	sigint.sa_flags = SA_SIGINFO;
	sigemptyset(&sigint.sa_mask);
	sigaction(SIGINT, &sigint, NULL);
	sigquit.sa_handler = SIG_IGN;
	sigquit.sa_flags = SA_SIGINFO;
	sigemptyset(&sigquit.sa_mask);
	sigaction(SIGQUIT, &sigquit, NULL);
}

void	ignore_sigint(void)
{
	struct sigaction	sigint;
	struct sigaction	sigquit;

	sigint.sa_handler = SIG_IGN;
	sigint.sa_flags = SA_SIGINFO;
	sigemptyset(&sigint.sa_mask);
	sigaction(SIGINT, &sigint, NULL);
	sigquit.sa_handler = SIG_IGN;
	sigquit.sa_flags = SA_SIGINFO;
	sigemptyset(&sigquit.sa_mask);
	sigaction(SIGQUIT, &sigquit, NULL);
}

void	handle_signals(int signal)
{
	if (signal == SIGINT)
	{
		g_last_ret_val = 130;
		ft_putchar_fd('\n', 1);
		rl_replace_line("\0", 0);
		rl_on_new_line();
		rl_redisplay();
		return ;
	}
	if (signal == SIGQUIT)
	{
		g_last_ret_val = 0;
		return ;
	}
}

void	exit_signal(t_list **cmd_table, t_env **env)
{
	(void)cmd_table;
	ft_envclear(env, &free);
	//need to free cmd_table
	printf("exit\n");
	exit(0);
}

int	split_free(char **str, int ret_val)
{
	size_t	i;

	i = 0;
	while (str[i] != NULL)
	{
		free(str[i]);
		i++;
	}
	free(str[i]);
	free(str);
	return (ret_val);
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
	printf("infiles: ");
	ft_lstiter(node->infiles, &print_tlist_string);
	printf("\n");
	printf("outfiles: ");
	ft_lstiter(node->outfiles, &print_tlist_string);
	printf("\n");
}

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
	while (true)
	{
		line = readline("\e[1;32m[MINISHELL]$> \e[0m");
		if (line)
		{
			cmd_table = init_cmd_table(line, env);
			if (cmd_table != NULL)
			{
				open_infiles(&cmd_table);
				//ft_lstiter(cmd_table, &print_cmd_list);
				g_last_ret_val = prepare_exec(cmd_table, &env);
			}
			add_history(line);
			free(line);
		}
		else
			exit_signal(&cmd_table, &env);
	}
}
