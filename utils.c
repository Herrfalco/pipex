#include "headers.h"

static t_bool		white_space(char c) {
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f');
}

static size_t		count_word(char *str) {
	size_t	res = 0;
	t_bool	space = TRUE;

	while (*str) {
		if (white_space(*str))
			space = TRUE;
		else {
			if (space)
				++res;
			space = FALSE;
		}
		++str;
	}
	return (res);
}

char		**split(char *str) {
	size_t	len = count_word(str);
	char	**res = malloc(sizeof(char *) * (len + 1));
	size_t	i = 0;
	t_bool	space = TRUE;

	if (!res)
		return (NULL);
	while (*str) {
		if (white_space(*str)) {
			space = TRUE;
			*str = '\0';
		} else {
			if (space)
				res[i++] = str;
			space = FALSE;
		}
		++str;
	}
	res[i] = NULL;
	return (res);
}

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
