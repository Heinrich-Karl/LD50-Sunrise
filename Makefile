.POSIX:

PROJ = ld50

SRC = ${PROJ}.c
OBJ = ${SRC:.c=.o}

LIB = ./lib

GL_SO = libOpenGL.so
GLFW_SO = libglfw.so.3
GLEW_SO = libGLEW.so.2.2
PNG_SO = libpng.so

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

${PROJ}: ${OBJ} ${LIB}/${GL_SO} ${LIB}/${GLFW_SO} ${LIB}/${GLEW_SO} ${LIB}/${PNG_SO}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

${LIB}/${GL_SO}:
	cp -f /lib/${GL_SO} ${LIB}

${LIB}/${GLFW_SO}:
	cp -f /lib/${GLFW_SO} ${LIB}

${LIB}/${GLEW_SO}:
	cp -f /lib/${GLEW_SO} ${LIB}

${LIB}/${PNG_SO}:
	cp -f /lib/${PNG_SO} ${LIB}


clean:
	rm -f ${PROJ} ${OBJ} ${LIB}/*

.PHONY: all options clean
