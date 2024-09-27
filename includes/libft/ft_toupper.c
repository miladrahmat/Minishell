/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 11:50:09 by mrahmat-          #+#    #+#             */
/*   Updated: 2024/05/08 10:31:36 by mrahmat-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_toupper(int c)
{
	if (c > 255)
		return (-1);
	if ((unsigned char)c >= 97 && (unsigned char)c <= 122)
		return (c - 32);
	else
		return (c);
}
