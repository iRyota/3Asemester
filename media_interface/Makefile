#MAKEFILE

CC=g++
RM=rm -f

CFLAGS= `pkg-config --cflags opencv` 
LDFLAGS= `pkg-config --libs opencv` -lglut -lGL -lGLU -ljpeg -lm -lpthread


all: TARGET1 TARGET2 TARGET3 TARGET4 TARGET5 TARGET6

TARGET1 = intCalib
OBJS1 = intCalib.o

TARGET2 = cvglsample
OBJS2 = cvglsample.o

TARGET3 = extCalib
OBJS3 = extCalib.o

TARGET4 = projection
OBJS4 = projection.o

TARGET5 = inverseProjection
OBJS5 = inverseProjection.o

TARGET6 = cvglview
OBJS6 = cvglview.o

.c.o:
	${CC} -c ${CFLAGS}  ${LDFLAGS} $<

TARGET1: ${OBJS1}
	${CC} -o ${TARGET1} ${OBJS1} ${LDFLAGS}

TARGET2: ${OBJS2}
	${CC} -o ${TARGET2} ${OBJS2} ${LDFLAGS}

TARGET3: ${OBJS3}
	${CC} -o ${TARGET3} ${OBJS3} ${LDFLAGS}

TARGET4: ${OBJS4}
	${CC} -o ${TARGET4} ${OBJS4} ${LDFLAGS}

TARGET5: ${OBJS5}
	${CC} -o ${TARGET5} ${OBJS5} ${LDFLAGS}

TARGET6: ${OBJS6}
	${CC} -o ${TARGET6} ${OBJS6} ${LDFLAGS}

clean:
	${RM} ${TARGET1} ${TARGET2} ${TARGET3} ${TARGET4} ${TARGET5} ${TARGET6} *.o *~


