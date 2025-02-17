/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars_process_var.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:38:40 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/26 15:15:23 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_questionmark(char **end, char **ret, t_var_expander extra_args)
{
	char	*value;
	char	*temp;

	value = ft_itoa(*extra_args.last_ret_val);
	if (!value)
		return (-1);
	temp = *ret;
	*ret = ft_strjoin_safe(*ret, value);
	if (temp)
		free(temp);
	free(value);
	if (!*ret)
		return (-1);
	*end = (*end) + 2;
	return (0);
}

int	process_env_var(char **end, char **ret, char *varname,
					t_var_expander extra_args)
{
	char	*temp;

	temp = *ret;
	*ret = ft_strjoin_safe(*ret, ft_env_get_value_by_key(
				varname, extra_args.env));
	if (temp)
		free(temp);
	if (!*ret)
		return (-1);
	*end = skip_varname(*end);
	return (0);
}
