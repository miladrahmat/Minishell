/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 15:28:18 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/05 16:56:30 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_quoted_str(char *s)
{
	if (s)
	{
		return ((s[0] == '\"' && s[ft_strlen(s) - 1] == '\"') ||
			(s[0] == '\'' && s[ft_strlen(s) - 1] == '\'')); 
	}
	return (false);
}

bool	is_double_quoted_str(char *s)
{
	if (s)
	{
		return ((s[0] == '\"' && s[ft_strlen(s) - 1] == '\"'));
	}
	return (false);
}

void	str_del_first_last(char *s)
{
	int	i;
	int	len;

	len = ft_strlen(s);
	i = 0;
	while (s[i] && s[i + 1])
	{
		s[i] = s[i + 1];
		i++;
	}
	s[len - 2] = 0;
}

// TODO: strip all quotes that are not inside of other quotes
void	strip_quotes(char *s)
{
	if (is_quoted_str(s))
		str_del_first_last(s);
}

void	strip_quotes2(char *s)
{
	char	*start;
	char	*end;
	char	*ret;
	int		quotes;
	char	*new_str;

	start = s;
	end = s;
	quotes = 0;
	ret = NULL;
	while (*start && *end)
	{
		while (*end)
		{
			if (quotes == 0)
			{
				if (*end == '\'')
				{
					quotes = 1;
					end++;
					break ;
				}
				else if (*end == '\"')
				{
					quotes = 2;
					end++;
					break ;
				}
			}
			else if (quotes == 1 && *end == '\'')
			{
				quotes = 0;
				end++;
				break ;
			}
			else if (quotes == 2 && *end == '\"')
			{
				quotes = 0;
				end++;
				break ;
			}
			end++;
		}
		new_str = get_token(start, end);
		ret = ft_strjoin(ret, new_str);
		start = end;
	}
	printf("strip_quotes2: %s\n", ret);
}
