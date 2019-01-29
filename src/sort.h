#ifndef SORT_H
#define SORT_H
#include <stdlib.h>
#include <string.h>

int qsort_s(void *b, size_t n, size_t s,
	int (*cmp)(const void *, const void *, void *), void *ctx);

#endif
