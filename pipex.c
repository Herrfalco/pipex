/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 12:32:18 by fcadet            #+#    #+#             */
/*   Updated: 2021/08/04 14:19:23 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.h"

static void	wait_pids(t_vec *pids)
{
	ssize_t		i;

	i = -1;
	while (++i < (ssize_t)pids->len)
		if (pids->data[i] >= 0)
			waitpid(pids->data[i], 0, 0);
}

static void	init_pipe(t_worker type, int n_pip[2], t_vec *pids)
{
	n_pip[0] = -1;
	n_pip[1] = -1;
	if (type != WRITER)
	{
		if (pipe(n_pip))
		{
			wait_pids(pids);
			free(pids->data);
			error(NULL, strerror(errno), 1, TRUE);
		}
	}
}

static int	fork_worker(t_worker type, char **av, char **env, t_vec *pids)
{
	static int	o_pip = -1;
	int			n_pip[2];
	pid_t		pid;

	init_pipe(type, n_pip, pids);
	if (!ass_ret(&pid, fork()))
	{
		free(pids->data);
		exec_worker(o_pip, n_pip, file_redir(type, &o_pip, n_pip, av), env);
	}
	close_in_out(o_pip, n_pip[1]);
	if (pid < 0)
	{
		wait_pids(pids);
		free(pids->data);
		close_in_out(n_pip[0], -1);
		error(NULL, "can't fork", 1, TRUE);
	}
	o_pip = n_pip[0];
	return (pid);
}

static void	main_loop(int ac, char **av, char **env, t_vec *pids)
{
	t_worker	type;
	ssize_t		i;

	i = 1;
	while (i < ac - 1)
	{
		type = EXECUTER;
		if (i == 1)
			type = READER;
		else if (i == ac - 2)
			type = WRITER;
		if (vec_add(pids, fork_worker(type, av + i, env, pids)))
		{
			wait_pids(pids);
			free(pids->data);
			error(NULL, "memory allocation error", 1, TRUE);
		}
		if (type == READER)
			i += 2;
		else
			i += 1;
	}
}

int	main(int ac, char **av, char **env)
{
	t_vec		pids;

	if (ac < 5)
		error(NULL, "wrong number of arguments", 1, TRUE);
	if (vec_init(&pids, BASE_CAP))
		error(NULL, "memory allocation error", 1, TRUE);
	main_loop(ac, av, env, &pids);
	wait_pids(&pids);
	free(pids.data);
	return (0);
}
