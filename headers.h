#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

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

char		**split(char *str);
int		vec_init(t_vec *vec, size_t cap);
int		vec_add(t_vec *vec, int val);
