.POSIX:

PROJ = ld50

SRC = ${PROJ}.c
OBJ = ${SRC:.c=.o}

INCS = -I/usr/include -I${HOME}/proj/include
LIBS = -lm -lGL -lglfw -lGLEW -lpng

CFLAGS	= ${INCS}
LDFLAGS	= ${LIBS}

CC = cc

all: options ${PROJ}

options:
	@echo openal_raw build options:
	@echo "CFLAGS 	= ${CFLAGS}"
	@echo "LDFLAGS	= ${LDFLAGS}"
	@echo "CC	= ${CC}"

.c.o:
	${CC} -c ${CFLAGS} $<

${OBJ}: 

${PROJ}: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f ${PROJ} ${OBJ}

.PHONY: all options clean
