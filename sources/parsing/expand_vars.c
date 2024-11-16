/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 15:27:49 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/16 17:48:45 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// return pointer to the last char copied from src
// replace dst
// NULL pointer is returned in case of malloc() failure in ft_strndup() or 
// ft_strjoin()
static char	*concatenate_until(char **dst, char *src, char *delim)
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
static int	reached_dollar(
	char **start, char **end, char **ret, t_var_expander extra_args)
{
	char	*varname;
	int		flag;

	if (!is_varname(*((*end) + 1)))
		return (not_varname(start, end, ret));
	varname = get_varname((*end) + 1);
	if (!varname)
		return (free_strs_int(ret, &varname));
	if (ft_strcmp(varname, "?") == 0)
		flag = process_questionmark(end, ret, extra_args);
	else
		flag = process_env_var(end, ret, varname, extra_args);
	if (flag < 0 || !*ret)
		return (free_strs_int(ret, &varname));
	free(varname);
	return (0);
}

static int	reached_single_quote(char **end, char **ret)
{
	char	*temp;

	temp = *ret;
	*ret = ft_strjoin(*ret, "'");
	if (temp)
		free(temp);
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
static char	*var_expander(char *token, t_var_expander extra_args)
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
