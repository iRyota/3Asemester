CC 		= g++

COMMON 		= .

RM 	= rm -f
TARGET 	= sample

OBJS 	= sample.o

CFLAGS  = -Wall -fpermissive
LDFLAGS = 
LDLIBS  = -lglut -lGL -lGLU -lcwiid -lrt -lbluetooth -lpthread


.c.o:
	${CC} -c ${CFLAGS} $<

${TARGET}: ${OBJS}
	${CC} -o ${TARGET} ${OBJS} ${LDLIBS} ${LDFLAGS}

clean:
	${RM} ${TARGET} *.o *~

