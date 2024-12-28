#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "htable.h"

static struct entry {
	const char *k;
	const char *v;
} *htable = NULL;

size_t k = 13;
size_t epk  = 1;

static unsigned long hash(char const *s)
{
	unsigned long hash = 0;
	int c;

	while ((c = *s++))
		hash = c + (hash << 6) + (hash << 16) - hash;

	return hash;
}

static int ecompar(void const *_a, void const *_b)
{
	struct entry *a = (struct entry *)_a;
	struct entry *b = (struct entry *)_b;
	if (!a->k || !b->k)
		return -1;
	return strcmp(a->k, b->k);
}

char const *lookup(char const *s)
{
	char *_z = strdup(s);
	if (!_z) return NULL;
	char *z = strtok(_z, WSPACE);
	unsigned long i = hash(z) % k;
	struct entry *entries = htable + i*epk;
	struct entry  entry   = { .k = z };
	struct entry *fin = bsearch(&entry, entries, epk, sizeof(*htable), ecompar);
	free(_z);
	if (fin)
		return fin->v;
	else
		return NULL;
}

static void resize(size_t n)
{
	size_t t = epk * k;
	epk = n;
	struct entry *new = realloc(htable, k * epk * sizeof(*htable));
	if (!new)
		return;
	htable = new;

	size_t i, c;
	for (i = c = epk - n; i < epk * k; i += epk, c += epk - n) {
		memmove(htable+i+n, htable+i, (t-c)*sizeof(*htable));
		memset(htable+i, 0, n*sizeof(*htable));
	}
}

static void _insert(char const *s, char const *v, unsigned long i)
{
	struct entry *entries = htable + i*epk;
	for (size_t i = 0; i < epk; i++) {
		if (!entries[i].k) {
			 entries[i] = (struct entry){
				.k = s,
				.v = v,
			};
			return;
		}
	}
	resize(epk*2);
	_insert(s, v, i);
}

void insert(char const *s, char const *v)
{
	unsigned long i = hash(s) % k;
	char *s2 = strdup(s), *v2 = strdup(v);
	/* no need to check these strdups. _insert is well behaved for NULL inputs */
	_insert(s2, v2, i);
}

void inittab(size_t ik, size_t iepk)
{
	k = ik;
	epk = iepk;
	htable = calloc(k * epk, sizeof(*htable));
}
