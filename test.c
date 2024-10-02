/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:35:34 by lemercie          #+#    #+#             */
/*   Updated: 2024/10/02 16:51:33 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./minishell.h"

void	print_list(void *arg)
{
	printf("%s\n", (char *) arg);
}

int	main(void)
{
	char	*line;
	t_list	*tokens;

	while (true)
	{
		line = readline("$>");
		if (line && *line)
		{
			if (ft_strncmp(line, "exit", 5) == 0)
			{
				free(line);
				break ;
			}
			tokens = tokenize(line);
			ft_lstiter(tokens, &print_list);
			add_history(line);
		}
		free(line);
	}
}
