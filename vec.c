#include "headers.h"

int		vec_init(t_vec *vec, size_t cap) {
	if (!(vec->data = malloc(sizeof(int) * cap)))
		return (-1);
	vec->len = 0;
	vec->cap = cap;
	return (0);
}

int		vec_add(t_vec *vec, int val) {
	int		*tmp;
	ssize_t		i = -1;

	if (vec->len + 1 > vec->cap) {
		if (!(tmp = malloc(sizeof(int) * vec->cap * 2)))
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
