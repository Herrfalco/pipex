#include "headers.h"

void	error(char *title, char *str, int ret, t_bool fat) {
	write(2, "pipex: ", 7);
	if (title) {
		write(2, title, str_len(title));
		write(2, ": ", 2);
	}
	write(2, str, str_len(str));
	write(2, "\n", 1);
	if (fat)
		exit(ret);
}

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
