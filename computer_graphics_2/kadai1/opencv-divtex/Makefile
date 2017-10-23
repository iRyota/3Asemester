#MAKEFILE

CC	= gcc
RM	= rm -f

CFLAGS	= -g -Wall -O2 `pkg-config opencv --cflags`
LDFLAGS = -lglut -lGLU -lGL -lm -lc `pkg-config opencv --libs`


TARGET	= videotexture
OBJS	= videotexture.o

.c.o:
	${CC} -c ${CFLAGS} $<

TARGET: ${OBJS}
	${CC} -o ${TARGET} ${OBJS} ${LDFLAGS}

clean:
	${RM} ${TARGET} *.o *~

