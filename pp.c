#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LDELIM "{{"
#define RDELIM "}}"

void expand(char *);
void eval(char *);

void eval(char *s)
{
	if (!s || (s && !*s))
		return;

	FILE *p = popen(s, "r");
	if (!p)
		return;

	char  *val = NULL;
	size_t len = 0;
	int endl = getdelim(&val, &len, EOF, p);

	pclose(p);
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
	while ((endl = getline(&line, &llen, stdin)) > 0)
		expand(line);
}
