#include "headers.h"

static int		path_init(t_path *path, char *root, char *fname, size_t cap) {
	path->fname = fname;
	path->cap = cap;
	if (!(path->data = malloc(sizeof(char) * path->cap)))
		return (-1);
	path->root = root;
	*path->data = '\0';
	return (0);
}

static int		path_next(t_path *path) {
	ssize_t		root_len = -1;
	ssize_t		i = -1;
	size_t		fact = 1;

	while (path->root && *path->root == ':')
		++path->root;
	if (!path->root || !*path->root)
		return (0);
	while (path->root[++root_len] && path->root[root_len] != ':');
	while (path->cap * fact < str_len(path->fname) + root_len + 2)
		fact *= 2;
	if (fact != 1) {
		free(path->data);
		if (!(path->data = malloc(sizeof(char) * path->cap * fact)))
			return (-1);
		path->cap *= fact;
	}
	while (++i < root_len)
		path->data[i] = path->root[i];
	path->root = path->root + root_len;
	path->data[root_len++] = '/';
	i = -1;
	while (path->fname[++i])
		path->data[root_len + i] = path->fname[i];
	path->data[root_len + i] = '\0';
	return (1);
}

int			find_exec(char **res, char *fname, char **env) {
	ssize_t		i = -1;
	int		next_ret = 0;
	t_path		path;

	while (env[++i]) {
		if (!strn_cmp(env[i], "PATH=", 5)) {
			if (path_init(&path, env[i] + 5, fname, BASE_CAP) < 0)
				return (-1);
			while ((next_ret = path_next(&path)) == 1) {
				if (!access(path.data, X_OK)) {
					*res = path.data;
					return (1);
				}
			}
			if (path.data)
				free(path.data);
		}
	}
	return (next_ret);
}
