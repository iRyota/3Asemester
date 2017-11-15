CC 		= gcc

COMMON 		= .

RM 	= rm -f
TARGET 	= cvglview_wii

OBJS 	= cvglview_wii.o

CFLAGS  = -O2 `pkg-config --cflags opencv`
LDFLAGS = 
LDLIBS  = `pkg-config --libs opencv` -lglut -lGL -lGLU -lcwiid -lrt -lbluetooth -lpthread -lm


.c.o:
	${CC} -c ${CFLAGS} $<

${TARGET}: ${OBJS}
	${CC} -o ${TARGET} ${OBJS} ${LDLIBS} ${LDFLAGS}

clean:
	${RM} ${TARGET} *.o *~

