#MAKEFILE

CC	= gcc
RM	= rm -f

CFLAGS	= -Wall	
LDFLAGS	= -lglut -lGLU -lGL -lm

TARGET	= sample-robot
OBJS	= sample-robot.o

.c.o:
	${CC} -c ${CFLAGS} $<

TARGET: ${OBJS}
	${CC} -o ${TARGET} ${OBJS} ${LDFLAGS}

clean:
	${RM} ${TARGET} *.o *~
