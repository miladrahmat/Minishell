/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 14:16:52 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/10/31 16:34:23 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	signal_handling_child(void)
{
	struct sigaction	sigint;
	struct sigaction	sigquit;

	sigint.sa_handler = SIG_DFL;
	sigint.sa_flags = SA_SIGINFO;
	sigemptyset(&sigint.sa_mask);
	sigaction(SIGINT, &sigint, NULL);
	sigquit.sa_handler = SIG_DFL;
	sigquit.sa_flags = SA_SIGINFO;
	sigemptyset(&sigquit.sa_mask);
	sigaction(SIGQUIT, &sigquit, NULL);
}

void	heredoc_signal(void *func)
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

void	exit_signal(t_list **cmd_table, t_env **env)
{
	(void)cmd_table;
	ft_envclear(env, &free);
	ft_lstclear(cmd_table, &destroy_tlist_of_tcmd);
	clear_history();
	printf("exit\n");
	exit(0);
}