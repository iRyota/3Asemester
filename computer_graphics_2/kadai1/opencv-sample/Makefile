#MAKEFILE

CC	= gcc
RM	= rm -f

CFLAGS	= -g -O2 `pkg-config opencv --cflags`
LDFLAGS = -lglut -lGLU -lGL -lm -lc `pkg-config opencv --libs`


TARGET	= videocapture
OBJS	= videocapture.o

.c.o:
	${CC} -c ${CFLAGS} $<

TARGET: ${OBJS}
	${CC} -o ${TARGET} ${OBJS} ${LDFLAGS}

clean:
	${RM} ${TARGET} *.o *~

