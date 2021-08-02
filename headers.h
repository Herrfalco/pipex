#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

#define BUFF_SIZE 	2048
#define BASE_CAP	1

typedef enum		e_worker {
	READER,
	EXECUTER,
	WRITER,
}			t_worker;

typedef enum		e_bool {
	FALSE,
	TRUE,
}			t_bool;

typedef struct		s_vec {
	int		*data;
	size_t		len;
	size_t		cap;
}			t_vec;

typedef struct		s_path {
	char		*data;
	char		*fname;
	char		*root;
	size_t		cap;
}			t_path;

size_t		str_len(char *str);
void		str_cpy(char *dst, char *src);
int		strn_cmp(char *s1, char *s2, size_t n);

void		error(char *title, char *str, int ret, t_bool fat);
char		**split(char *str);

int		vec_init(t_vec *vec, size_t cap);
int		vec_add(t_vec *vec, int val);

int		find_exec(char **res, char *fname, char **env);
