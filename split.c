/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 14:27:32 by fcadet            #+#    #+#             */
/*   Updated: 2021/08/06 11:59:50 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.h"

static t_bool	white_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v'
		|| c == '\f');
}

static size_t	count_word(char *str)
{
	size_t	res;
	t_bool	space;

	res = 0;
	space = TRUE;
	while (*str)
	{
		if (white_space(*str))
			space = TRUE;
		else
		{
			if (space)
				++res;
			space = FALSE;
		}
		++str;
	}
	return (res);
}

char	**split_init(char *str, size_t *len, size_t *i, t_bool *space)
{
	*i = 0;
	*space = TRUE;
	*len = count_word(str);
	return (malloc(sizeof(char *) * (*len + 1)));
}

char	**split(char *str)
{
	size_t	len;
	char	**res;
	size_t	i;
	t_bool	space;

	res = split_init(str, &len, &i, &space);
	if (!res)
		return (res);
	while (*str)
	{
		if (white_space(*str))
		{
			space = TRUE;
			*str = '\0';
		}
		else if (space)
		{
			res[i++] = str;
			space = FALSE;
		}
		++str;
	}
	res[i] = NULL;
	return (res);
}
