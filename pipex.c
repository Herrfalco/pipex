#include "headers.h"

#define BUFF_SIZE 	2048

size_t	str_len(char *str) {
	size_t		len = 0;

	while(*(str++))
		++len;
	return (len);
}

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

void	close_in_out(int in, int out) {
	if (in != -1)
		close(in);
	if (out != -1)
		close(out);
}

void	exec_reader(int pipe[2], char *arg) {
	int		input;
	char		buff[BUFF_SIZE];
	ssize_t		read_ret;
	ssize_t		write_ret;

	close(pipe[0]);
	if ((input = open(arg, O_RDONLY)) < 0) {
		close(pipe[1]);
		error(arg, strerror(errno), 1, TRUE);
	}
	while ((read_ret = read(input, buff, BUFF_SIZE)) > 0) {
		if ((write_ret = write(pipe[1], buff, read_ret)) < 0) {
			close_in_out(input, pipe[1]);
			error(NULL, strerror(errno), 1, TRUE);
		}
	}
	close_in_out(input, pipe[1]);
	if (read_ret < 0)
		error(arg, strerror(errno), 1, TRUE);
	exit(0);
}

void	exec_executer(int input, int pipe[2], char **args, char **env) {
	close(pipe[0]);
	dup2(input, 0);
	dup2(pipe[1], 1);
	if (execve(*args, args, env)) {
		close_in_out(input, pipe[1]);
		error(*args, "command not found", 1, FALSE);
		free(args);
		exit(1);
	}
}

void	exec_writer(int input, char *arg) {
	int		output;
	char		buff[BUFF_SIZE];
	ssize_t		read_ret;
	ssize_t		write_ret;

	if ((output = open(arg, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0) {
		close(input);	
		error(arg, strerror(errno), 1, TRUE);
	}
	while ((read_ret = read(input, buff, BUFF_SIZE)) > 0) {
		if ((write_ret = write(output, buff, read_ret)) < 0) {
			close_in_out(input, output);
			error(NULL, strerror(errno), 1, TRUE);
		}
	}
	close_in_out(input, output);
	if (read_ret < 0)
		error(arg, strerror(errno), 1, TRUE);
	exit(0);
}

int	exec_cmd(t_worker type, char *arg, char **env, t_vec *pids) {
	static int	old_pipe = -1;
	int		new_pipe[2] = { -1, -1 };
	char		**args;
	pid_t		pid;

	
	if (type != WRITER) {
		if (pipe(new_pipe))
			error(NULL, strerror(errno), 1, TRUE);
	}
	if (!(pid = fork())) {
		free(pids->data);
		if (type == READER) {
			exec_reader(new_pipe, arg);
		} else if (type == WRITER) {
			exec_writer(old_pipe, arg);
		} else {
			if (!(args = split(arg))) {
				close_in_out(old_pipe, new_pipe[1]);
				error(NULL, strerror(errno), 1, TRUE);
			}
			exec_executer(old_pipe, new_pipe, args, env);
		}
	}
	close_in_out(old_pipe, new_pipe[1]);
	if (pid < 0) {
		close(new_pipe[0]);
		error(NULL, "can't fork", 1, TRUE);
	}
	old_pipe = new_pipe[0];
	return (pid);
}

int	main(int ac, char **av, char **env) {
	t_worker	type;
	t_vec		pids;
	int		i = 0;
	ssize_t		j;

	if (ac < 5)
		error(NULL, "wrong number of arguments", 1, TRUE);
	if (vec_init(&pids, 5))
		error(NULL, "memory allocation error", 1, TRUE);
	while (av[++i]) {
		if (i == 1)
			type = READER;
		else if (i == ac - 1)
			type = WRITER;
		else
			type = EXECUTER;
		if (vec_add(&pids, exec_cmd(type, av[i], env, &pids))) {
			j = -1;
			while (++j < (ssize_t)pids.len)
				waitpid(pids.data[j], 0, 0);
			free(pids.data);
			error(NULL, "memory allocation error", 1, TRUE);
		}
	}
	j = -1;
	while (++j < (ssize_t)pids.len)
		waitpid(pids.data[j], 0, 0);
	free(pids.data);
	return (0);
}
