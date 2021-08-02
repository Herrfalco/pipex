#include "headers.h"

static void	close_in_out(int in, int out) {
	if (in != -1)
		close(in);
	if (out != -1)
		close(out);
}

static void	exec(int input, int pipe[2], char **args, char **env) {
	char	*path;
	int	search_res;

	close_in_out(pipe[0], -1);
	dup2(input, 0);
	dup2(pipe[1], 1);
	if ((search_res = find_exec(&path, *args, env)) < 0) {
		close_in_out(input, pipe[1]);
		error(NULL, "memory allocation error", 1, FALSE);
	} else if (!search_res || execve(path, args, env)) {
		close_in_out(input, pipe[1]);
		error(*args, "command not found", 1, FALSE);
	}
	free(args);
	exit(1);
}

static int	exec_cmd(t_worker type, char **av, char **env, t_vec *pids) {
	static int	old_pipe = -1;
	int		new_pipe[2] = { -1, -1 };
	char		**args;
	pid_t		pid;

	
	if (type != WRITER) {
		if (pipe(new_pipe)) {
			free(pids->data);
			error(NULL, strerror(errno), 1, TRUE);
		}
	}
	if (!(pid = fork())) {
		free(pids->data);
		if (!(args = split(type == READER ? av[1] : av[0]))) {
			close_in_out(old_pipe, new_pipe[1]);
			error(NULL, strerror(errno), 1, TRUE);
		}
		if (type == READER) {
			if ((old_pipe = open(av[0], O_RDONLY)) < 0) {
				free(args);
				close_in_out(new_pipe[1], -1);
				error(av[0], strerror(errno), 1, TRUE);
			}
		} else if (type == WRITER) {
			if ((new_pipe[1] = open(av[1], O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0) {
				free(args);
				close_in_out(old_pipe, -1);	
				error(av[1], strerror(errno), 1, TRUE);
			}
		}
		exec(old_pipe, new_pipe, args, env);
	}
	close_in_out(old_pipe, new_pipe[1]);
	if (pid < 0) {
		free(pids->data);
		close_in_out(new_pipe[0], -1);
		error(NULL, "can't fork", 1, TRUE);
	}
	old_pipe = new_pipe[0];
	return (pid);
}

int	main(int ac, char **av, char **env) {
	t_worker	type;
	t_vec		pids;
	int		i = 1;
	ssize_t		j;

	if (ac < 5)
		error(NULL, "wrong number of arguments", 1, TRUE);
	if (vec_init(&pids, BASE_CAP))
		error(NULL, "memory allocation error", 1, TRUE);
	while (i < ac - 1) {
		if (i == 1)
			type = READER;
		else if (i == ac - 2)
			type = WRITER;
		else
			type = EXECUTER;
		if (vec_add(&pids, exec_cmd(type, av + i, env, &pids))) {
			j = -1;
			while (++j < (ssize_t)pids.len)
				waitpid(pids.data[j], 0, 0);
			free(pids.data);
			error(NULL, "memory allocation error", 1, TRUE);
		}
		i = type == READER ? i + 2 : i + 1;
	}
	j = -1;
	while (++j < (ssize_t)pids.len)
		waitpid(pids.data[j], 0, 0);
	free(pids.data);
	return (0);
}
