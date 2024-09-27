/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:35:34 by lemercie          #+#    #+#             */
/*   Updated: 2024/09/27 15:14:44 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./minishell.h"

int	main(void)
{
	char	*line;

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
			printf("%s\n", line);
			add_history(line);
		}
		free(line);
	}
}

