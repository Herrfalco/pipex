/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 13:56:52 by fcadet            #+#    #+#             */
/*   Updated: 2021/08/04 13:41:06 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.h"

static int	path_init(t_path *path, char *root, char *fname, size_t cap)
{
	path->fname = fname;
	path->cap = cap;
	path->data = malloc(sizeof(char) * path->cap);
	if (!path->data)
		return (-1);
	path->root = root;
	*path->data = '\0';
	return (0);
}

static int	path_ralloc(t_path *path, ssize_t root_len)
{
	size_t		fact;

	fact = 1;
	while (path->cap * fact < str_len(path->fname) + root_len + 2)
		fact *= 2;
	if (fact != 1)
	{
		free(path->data);
		path->data = malloc(sizeof(char) * path->cap * fact);
		if (!path->data)
			return (-1);
		path->cap *= fact;
	}
	return (0);
}

static int	path_next(t_path *path)
{
	ssize_t		root_len;
	ssize_t		i;

	root_len = -1;
	i = -1;
	while (path->root && *path->root == ':')
		++path->root;
	if (!path->root || !*path->root)
		return (0);
	while (path->root[++root_len] && path->root[root_len] != ':')
		;
	if (path_ralloc(path, root_len) < 0)
		return (-1);
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

int	find_exec(char **res, char *fname, char **env)
{
	ssize_t		i;
	int			next_ret;
	t_path		path;

	i = -1;
	next_ret = 0;
	while (env[++i])
	{
		if (!strn_cmp(env[i], "PATH=", 5))
		{
			if (path_init(&path, env[i] + 5, fname, BASE_CAP) < 0)
				return (-1);
			while (ass_ret(&next_ret, path_next(&path)) == 1)
			{
				if (!access(path.data, X_OK))
				{
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
