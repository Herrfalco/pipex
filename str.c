#include "headers.h"

size_t		str_len(char *str) {
	size_t		len = 0;

	while(*(str++))
		++len;
	return (len);
}

void		str_cpy(char *dst, char *src) {
	while (*src)
		*(dst++) = *(src++);
}

int		strn_cmp(char *s1, char *s2, size_t n) {
	while (*s1 == *(s2++) && *(s1++) && n--);
	return (n ? *s2 - *s1 : 0);
}
