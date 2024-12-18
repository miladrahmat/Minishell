/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_tokens.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 16:22:58 by lemercie          #+#    #+#             */
/*   Updated: 2024/11/22 15:12:01 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_lstadd_middle(t_list **start, t_list *to_add)
{
	t_list	*trail;

	trail = (*start)->next;
	(*start)->next = to_add;
	ft_lstadd_back(&to_add, trail);
}

static int	split_expanded_token( \
	t_list *split_tokens_iter, char *expanded_token)
{
	t_list	*split_again;

	if (split_tokens_iter->content)
		free(split_tokens_iter->content);
	split_again = split_token(expanded_token);
	free(expanded_token);
	if (!split_again || !split_again->content)
		return (1);
	split_tokens_iter->content = ft_strdup(split_again->content);
	if (!split_tokens_iter->content)
		return (1);
	if (ft_lstsize(split_again) == 1)
		ft_lstdelone(split_again, free);
	else if (split_again->next)
	{
		ft_lstadd_middle(&split_tokens_iter, split_again->next);
		ft_lstdelone(split_again, free);
	}
	return (0);
}

// return 1 on malloc fails
int	transform_tokens1(t_list **head, t_env *env, int *last_ret_val)
{
	t_list	*split_tokens_iter;
	char	*expanded_token;

	split_tokens_iter = *head;
	while (split_tokens_iter)
	{
		expanded_token = expand_vars(split_tokens_iter->content,
				env, last_ret_val);
		if (!expanded_token)
			return (1);
		if (ft_strlen(expanded_token) == 0)
		{
			ft_lstdel_and_connect(head, &split_tokens_iter);
			free(expanded_token);
		}
		else
		{
			if (split_expanded_token(split_tokens_iter, expanded_token) == 1)
				return (1);
		}
		if (split_tokens_iter)
			split_tokens_iter = split_tokens_iter->next;
	}
	return (0);
}

// return 1 on malloc fails
int	transform_tokens2(t_list **head, int *last_ret_val)
{
	t_list	*split_tokens_iter;
	char	*unquoted_token;

	split_tokens_iter = *head;
	while (split_tokens_iter)
	{
		unquoted_token = strip_quotes((char *) split_tokens_iter->content,
				last_ret_val);
		if (!unquoted_token)
			return (1);
		if (split_tokens_iter->content)
			free(split_tokens_iter->content);
		split_tokens_iter->content = unquoted_token;
		if (split_tokens_iter)
			split_tokens_iter = split_tokens_iter->next;
	}
	return (0);
}
