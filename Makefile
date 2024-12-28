.POSIX:

TOPDIR != pwd
SRC    != find $(TOPDIR) -name "*.c"
OBJ   ::= $(SRC:.c=.o)

CC     = /bin/cc
CFLAGS = -O2 -g -pedantic -Wall -Wextra

test : res
	./test.sh

res : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

.c.o :
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	rm -f $(OBJ)
