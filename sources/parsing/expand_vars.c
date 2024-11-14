/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 15:27:49 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/14 15:17:04 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// return pointer to the last char copied from src
// replace dst
// NULL pointer is returned in case of malloc() failure in ft_strndup() or 
// ft_strjoin()
char	*concatenate_until(char **dst, char *src, char *delim)
{
	char	*src_end;
	char	*temp;
	char	*temp_joined;

	src_end = src;
	while (*src_end && !ft_strchr(delim, *src_end))
	{
		src_end++;
	}
	temp = ft_strndup(src, substr_len(src, src_end));
	if (!temp)
		return (NULL);
	temp_joined = ft_strjoin(*dst, temp);
	free(temp);
	if (!temp_joined)
		return (NULL);
	free(*dst);
	*dst = temp_joined;
	return (src_end);
}

// return -1 on malloc fail
// return 1 ==> use continue in caller
int	reached_dollar(
	char **start, char **end, char **ret, t_var_expander extra_args)
{
	char	*varname;
	char	*value;
	char	*temp;

	if (!is_varname(*((*end) + 1)))
		return (not_varname(start, end, ret));
	varname = get_varname((*end) + 1);
	if (!varname)
		return (free_strs_int(ret, &varname));
	if (ft_strcmp(varname, "?") == 0)
	{
		value = ft_itoa(*extra_args.last_ret_val);
		if (!value)
			return (free_strs_int(ret, &varname));
		temp = *ret;
		*ret = ft_strjoin_safe(*ret, value);
		free(temp);
		free(value);
		if (!*ret)
			return (free_strs_int(ret, &varname));
		*end = (*end) + 2;
	}
	else
	{
		temp = *ret;
		*ret = ft_strjoin_safe(*ret, ft_env_get_value_by_key(
					varname, extra_args.env));
		free(temp);
		if (!*ret)
			return (free_strs_int(ret, &varname));
		*end = skip_varname(*end);
	}
	if (!*ret)
		return (free_strs_int(ret, &varname));
	free(varname);
	return (0);
}

int	reached_single_quote(char **end, char **ret)
{
	*ret = ft_strjoin(*ret, "'");
	if (!*ret)
		return (-1);
	*end = concatenate_until(ret, (*end) + 1, "'");
	if (!*end)
		return (-1);
	return (0);
}

// join until $ or single quote (that is not inside of doubles)
// if $, get word and expand
// if single quote join until next single quote

// creates a new string where $VAR in the token is replaced by the value of
// $VAR in env
// token is NOT freed here because it is contained in a list node in the caller
// returns NULL in case of malloc fails
char	*var_expander(char *token, t_var_expander extra_args)
{
	char	*ret;
	char	*end;
	int		flag;

	ret = NULL;
	while (*token)
	{
		end = concatenate_until(&ret, token, "$'");
		if (!end || !ret)
			return (free_strs(&ret, NULL));
		if (*end == '$')
		{
			flag = reached_dollar(&token, &end, &ret, extra_args);
			if (flag < 0)
				return (free_strs(&ret, NULL));
			if (flag > 0)
				continue ;
		}
		else if (*end == '\'' && reached_single_quote(&end, &ret) < 0)
			return (free_strs(&ret, NULL));
		token = end;
	}
	return (ret);
}

char	*expand_vars(char *token, t_env *env, int *last_ret_val)
{
	t_var_expander	extra_args;

	extra_args.env = env;
	extra_args.last_ret_val = last_ret_val;
	if (!token)
		return (NULL);
	else if (*token == '\'')
		return (ft_strdup(token));
	else
		return (var_expander(token, extra_args));
}
