SRC = ${wildcard *.c}
DIR = ${notdir ${SRC}}
FOLDER = ./snapshot
CC = gcc
CFLAGS = -lpthread
TARGET = yoyoyo


${TARGET}: ${DIR}
	${CC} -o $@ ${DIR} ${CFLAGS}

clean:
	rm -rf ${FOLDER}/*.jpg ${TARGET}
