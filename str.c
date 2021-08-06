/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 14:30:51 by fcadet            #+#    #+#             */
/*   Updated: 2021/08/03 14:30:52 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.h"

size_t	str_len(char *str)
{
	size_t	len;

	len = 0;
	while (*(str++))
		++len;
	return (len);
}

void	str_cpy(char *dst, char *src)
{
	while (*src)
		*(dst++) = *(src++);
}

int	strn_cmp(char *s1, char *s2, size_t n)
{
	while (*s1 == *(s2++) && *(s1++) && n--)
		;
	if (n)
		return (*s2 - *s1);
	else
		return (0);
}
