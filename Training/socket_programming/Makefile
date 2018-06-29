src = ${wildcard *.c}
dir = ${notdir ${src}}
obj = ${patsubst %.c, %, ${dir}}
CC = gcc 
CFLAGS = -Wall

all: ${obj}

${obj}: ${%.c}
	${CC} ${CFLAGS} -o $@ $@.c

clean:
	rm ${obj}
