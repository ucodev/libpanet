CC=`cat ../.compiler`
CCFLAGS=-fPIC -fstrict-aliasing -O2 -Wall -Werror -pedantic
ECFLAGS=`cat ../.ecflags`
LDFLAGS=-shared -s
ELFLAGS=`cat ../.elflags`
TARGET_BIN=libpanet.so

all:
	${CC} ${CCFLAGS} ${ECFLAGS} -c client.c
	${CC} ${CCFLAGS} ${ECFLAGS} -c info.c
	${CC} ${CCFLAGS} ${ECFLAGS} -c low.c
	${CC} ${CCFLAGS} ${ECFLAGS} -c mm.c
	${CC} ${CCFLAGS} ${ECFLAGS} -c server.c
	${CC} ${CCFLAGS} ${ECFLAGS} -c timeout.c
	${CC} ${LDFLAGS} -o ${TARGET_BIN} client.o info.o low.o mm.o server.o timeout.o ${ELFLAGS}

clean:
	rm -f *.o
	rm -f ${TARGET_BIN}

