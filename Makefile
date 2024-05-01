CC = gcc
CFLAGS = -Wall -Wextra -Werror -ansi -pedantic
VALFLAGS = --track-origins=yes --leak-check=full \
					 --show-leak-kinds=all --trace-children=yes
ARG ?= test

all: htable

val: htable
	valgrind ${VALFLAGS} ./htable ${ARG}

htable: main.o htable.o
	${CC} main.o htable.o -o htable

main.o: main.c htable.h
	${CC} -c -g ${CFLAGS} main.c

htable.o: htable.c htable.h
	${CC} -c -g ${CFLAGS} htable.c

clean:
	rm -f htable main.o htable.o



