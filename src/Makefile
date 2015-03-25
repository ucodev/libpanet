CC=`cat ../.compiler`
INCLUDEDIRS=-I../include
CCFLAGS=
ECFLAGS=`cat ../.ecflags`
LDFLAGS=-shared -s
ELFLAGS=`cat ../.elflags`
ARCHFLAGS=`cat ../.archflags`
TARGET_BIN=libpanet.`cat ../.extlib`

all:
	${CC} ${INCLUDEDIRS} ${CCFLAGS} ${ECFLAGS} ${ARCHFLAGS} -c client.c
	${CC} ${INCLUDEDIRS} ${CCFLAGS} ${ECFLAGS} ${ARCHFLAGS} -c info.c
	${CC} ${INCLUDEDIRS} ${CCFLAGS} ${ECFLAGS} ${ARCHFLAGS} -c low.c
	${CC} ${INCLUDEDIRS} ${CCFLAGS} ${ECFLAGS} ${ARCHFLAGS} -c mm.c
	${CC} ${INCLUDEDIRS} ${CCFLAGS} ${ECFLAGS} ${ARCHFLAGS} -c server.c
	${CC} ${INCLUDEDIRS} ${CCFLAGS} ${ECFLAGS} ${ARCHFLAGS} -c timeout.c
	${CC} ${LDFLAGS} -o ${TARGET_BIN} client.o info.o low.o mm.o server.o timeout.o ${ELFLAGS}

clean:
	rm -f *.o
	rm -f ${TARGET_BIN}

