#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WSPACE " \t\n"
#define DEFINE '@'
#define LDELIM "{{"
#define RDELIM "}}"

void expand(char *);
void eval(char *);

void define(char *s)
{
	//interpret s as a definition and add it to the table
}

char *lookup(char *s)
{
	return NULL;
}

int gt2tok(char *s)
{
	return 1;
}

void sheval(char *s, char **buf, size_t *lbuf)
{
	FILE *p = popen(s, "r");
	if (!p)
		return;
	int endl = getdelim(buf, lbuf, EOF, p);
	pclose(p);
}

void eval(char *s)
{
	if (!s || (s && !*s))
		return;

	char  *val = NULL;
	size_t len = 0;
	if (gt2tok(s) || !(val = lookup(s))) {
		sheval(s, &val, &len);
	} else if (val) {
		val = strdup(val);
	}
	expand(val);
	free(val);
}

void expand(char *s)
{
	if (!s || (s && !*s))
		return;
	char *strt = strstr(s, LDELIM);
	char *end  = "";
	if (strt) {
		*strt = '\0';
		strt += sizeof(LDELIM);
		end = strstr(strt, RDELIM);
		if (end) {
			*end = '\0';
			end += sizeof(RDELIM);
		}
	}
	fputs(s, stdout);
	eval(strt);
	expand(end);
}

int main(void)
{
	int endl;
	char  *line = NULL;
	size_t llen = 0;
	while ((endl = getline(&line, &llen, stdin)) > 0) {
		char *strt = line + strspn(line, WSPACE);
		if (*strt == DEFINE)
			define(strt+1);
		else
			expand(strt);
	}
}
