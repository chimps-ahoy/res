#include <stdlib.h>
#include <string.h>
#include "htable.h"

static struct {
	const char *k;
	const char *v;
} *htable = NULL;

size_t k = 13;
size_t epk  = 1;

char const *lookup(char const *k)
{
	return NULL;
}

void insert(char const *k, char const *v)
{
	/* UNIMPLEMENTED */
}

static void resize(size_t n)
{
	size_t t = epk * k;
	epk += n;
	htable = realloc(htable, k * epk * sizeof(*htable));

	size_t i, c;
	for (i = c = epk - n; i < epk * k; i += epk, c += epk - n) {
		memmove(htable+i+n, htable+i, (t-c)*sizeof(*htable));
		memset(htable+i, 0, n*sizeof(*htable));
	}
}

void inittab(size_t ik, size_t iepk)
{
	k = k;
	epk = iepk;
	htable = calloc(k * epk, sizeof(*htable));
}
