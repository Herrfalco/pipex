/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   headers.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 13:27:31 by fcadet            #+#    #+#             */
/*   Updated: 2021/08/04 13:55:29 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADERS_H
# define HEADERS_H

# include <sys/wait.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdlib.h>
# include <errno.h>
# include <string.h>
# include <fcntl.h>

# define BUFF_SIZE 	1
# define BASE_CAP	1

typedef enum e_worker {
	READER,
	EXECUTER,
	WRITER,
}	t_worker;

typedef enum e_bool {
	FALSE,
	TRUE,
}	t_bool;

typedef struct s_marg {
	int			ac;
	char		**av;
	char		**env;
}	t_marg;

typedef struct s_vec {
	int			*data;
	size_t		len;
	size_t		cap;
}	t_vec;

typedef struct s_path {
	char		*data;
	char		*fname;
	char		*root;
	size_t		cap;
}	t_path;

char		**file_redir(t_worker type, int *o_pip, int n_pip[2], char **av);
void		exec_worker(int input, int pipe[2], char **args, char **env);

size_t		str_len(char *str);
void		str_cpy(char *dst, char *src);
int			strn_cmp(char *s1, char *s2, size_t n);

void		close_in_out(int in, int out);
void		error(char *title, char *str, int ret, t_bool fat);
int			ass_ret(int *dst, int src);

char		**split(char *str);

int			vec_init(t_vec *vec, size_t cap);
int			vec_add(t_vec *vec, int val);

int			find_exec(char **res, char *fname, char **env);

#endif
