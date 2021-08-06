/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 13:20:11 by fcadet            #+#    #+#             */
/*   Updated: 2021/08/06 13:26:28 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.h"

static void	open_2_read(int *o_pip, int n_pip[2], char *fname, char **args)
{
	if (ass_ret(o_pip, open(fname, O_RDONLY)) < 0)
	{
		free(args);
		close_in_out(n_pip[1], -1);
		error(fname, strerror(errno), 1, TRUE);
	}
}

static void	open_2_write(int o_pip, int n_pip[2], char *fname, char **args)
{
	if (ass_ret(&n_pip[1], open(fname, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0)
	{
		free(args);
		close_in_out(o_pip, -1);
		error(fname, strerror(errno), 1, TRUE);
	}
}

char	**file_redir(t_worker type, int *o_pip, int n_pip[2], char **av)
{
	char		**args;

	if (type == READER)
		args = split(av[1]);
	else
		args = split(av[0]);
	if (!args)
	{
		close_in_out(*o_pip, n_pip[1]);
		error(NULL, strerror(errno), 1, TRUE);
	}
	if (type == READER)
		open_2_read(o_pip, n_pip, av[0], args);
	else if (type == WRITER)
		open_2_write(*o_pip, n_pip, av[1], args);
	return (args);
}

void	exec_worker(int input, int pipe[2], char **args, char **env)
{
	char	*path;
	int		search_res;

	close_in_out(pipe[0], -1);
	dup2(input, 0);
	dup2(pipe[1], 1);
	if (ass_ret(&search_res, find_exec(&path, *args, env)) < 0)
	{
		close_in_out(input, pipe[1]);
		error(NULL, "memory allocation error", 1, FALSE);
	}
	else if (!search_res || execve(path, args, env))
	{
		close_in_out(input, pipe[1]);
		error(*args, "command not found", 1, FALSE);
	}
	free(args);
	exit(1);
}
