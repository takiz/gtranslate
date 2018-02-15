# gtranslate

CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -lcurl

SRC = gtranslate.c

gtranslate:
	${CC} ${CFLAGS} ${SRC} -o $@ ${LIBS}

clean:
	rm -f *.o gtranslate
