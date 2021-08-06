/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 14:32:15 by fcadet            #+#    #+#             */
/*   Updated: 2021/08/04 13:54:54 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.h"

int	vec_init(t_vec *vec, size_t cap)
{
	vec->data = malloc(sizeof(int) * cap);
	if (!vec->data)
		return (-1);
	vec->len = 0;
	vec->cap = cap;
	return (0);
}

int	vec_add(t_vec *vec, int val)
{
	int			*tmp;
	ssize_t		i;

	i = -1;
	if (vec->len + 1 > vec->cap)
	{
		tmp = malloc(sizeof(int) * vec->cap * 2);
		if (!tmp)
			return (-1);
		vec->cap *= 2;
		while (++i < (ssize_t)vec->len)
			tmp[i] = vec->data[i];
		free(vec->data);
		vec->data = tmp;
	}
	vec->data[vec->len++] = val;
	return (0);
}
