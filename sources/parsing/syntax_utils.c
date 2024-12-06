/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 16:43:29 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/12/06 16:43:42 by mrahmat-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_special_chars(char redir_type)
{
	char	*special_chars;

	if (redir_type == '>')
		special_chars = "<|";
	else
		special_chars = ">|";
	return (special_chars);
}
