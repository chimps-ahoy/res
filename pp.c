#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "htable.h"

#define DEFINE '@'
#define LDELIM "{{"
#define RDELIM "}}"

static void expand(char *);
static void eval(char *);

#define SPLIT(s,z) \
do {\
	s += strspn(s, WSPACE);\
	z = s + strcspn(s, WSPACE);\
	z += strspn(z, WSPACE);\
} while (0)

static void define(char *s)
{
	char *z;
	SPLIT(s, z);
	*(s + strcspn(s, WSPACE)) = 0;
	insert(s, z);
}

static int gt2tok(char *s)
{
	char *z;
	SPLIT(s, z);
	return s != z && *z;
}
#undef SPLIT

static void sheval(char *s, char **buf, size_t *lbuf)
{
	FILE *p = popen(s, "r");
	if (!p)
		return;
	(void)getdelim(buf, lbuf, EOF, p);

	/* This causes us to discard the output of the command if it exits with
	 * _anything_ other than exit code 0, which _could_ be problematic.
	 *
	 * the only solution is to hand roll something with fork(2),
	 * exec*(3)/system(3), and/or pipe(2) and dup2(2), but that is a lot of
	 * work so i might do that later if i ever do
	 *
	 */
	if (pclose(p))
		memset(*buf, 0, *lbuf);
}

static void eval(char *s)
{
	if (!s || !*s)
		return;

	char  *val = NULL;
	size_t len = 0;
	if (gt2tok(s) || !(val = lookup(s))) {
		sheval(s, &val, &len);
	} else if (val) {
		char *tmp = strdup(val);
		if (!tmp)
			fputs(val, stdout);
		else
			val = tmp;
	}
	expand(val);
	free(val);
}

static void expand(char *s)
{
	if (!s || !*s)
		return;

	char *strt = strstr(s, LDELIM);
	char *end  = "";
	if (strt) {
		*strt = '\0';
		strt += sizeof(LDELIM) - 1;
		end = strstr(strt, RDELIM);
		if (end) {
			*end = '\0';
			end += sizeof(RDELIM) - 1;
		}
	}
	fputs(s, stdout);
	eval(strt);
	expand(end);
}

int main(void)
{
	if (!inittab(13, 1))
		return EXIT_FAILURE;
	ssize_t endl;
	char  *line = NULL;
	size_t llen = 0;
	while ((endl = getline(&line, &llen, stdin)) > 0) {
		char *strt = line + strspn(line, WSPACE);
		if (*strt == DEFINE) {
			line[endl - 1] = 0;
			define(strt + 1);
		} else {
			expand(strt);
		}
	}
	return EXIT_SUCCESS;
}
