/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars_helpers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 14:18:23 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/22 14:29:36 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*skip_varname(char *s)
{
	if (!s)
		return (NULL);
	if (*s == '$')
		s++;
	while (ft_isalnum(*s) || *s == '_')
	{
		s++;
	}
	return (s);
}

char	*get_varname(char *start)
{
	char	*end;

	if (start[0] == '?')
	{
		return (ft_strdup("?"));
	}
	end = skip_varname(start);
	return (ft_strndup(start, substr_len(start, end)));
}

bool	is_varname(char c)
{
	return (ft_isalnum(c) || c == '_' || c == '?');
}

int	not_varname(char **start, char **end, char **ret)
{
	char	*temp;

	(*end)++;
	if (!(**end))
	{
		temp = *ret;
		*ret = ft_strjoin(*ret, "$");
		free(temp);
		if (!*ret)
			return (-1);
	}
	else if (!**end || is_whitespace(**end) || **end == '\"')
	{
	//	if (!(*end)[1] || is_whitespace((*end)[1]))
	//	{
			temp = *ret;
			*ret = ft_strjoin(*ret, "$");
			free(temp);
			if (!*ret)
				return (-1);
	//	}
	}
	*start = *end;
	return (1);
}

int	expand_vars_in_filenames(t_cmd *cmd, t_env *env, int *last_ret_val)
{
	t_list	*files_iter;
	t_redir	*redir;
	char	*expanded_filename;

	files_iter = cmd->files;
	while (files_iter)
	{
		redir = files_iter->content;
		if (redir->redir_type != heredoc)
		{
			expanded_filename = expand_vars(redir->filename, env, last_ret_val);
			if (!expanded_filename)
				return (1);
			if (redir->filename)
				free(redir->filename);
			redir->filename = expanded_filename;
		}
		files_iter = files_iter->next;
	}
	return (0);
}
