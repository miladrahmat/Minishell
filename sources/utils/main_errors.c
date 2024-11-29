/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_errors.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 16:38:26 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/29 16:39:06 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	line_has_syntax_error(char *line, int *last_ret_val)
{
	*last_ret_val = 2;
	add_history(line);
	free(line);
}
