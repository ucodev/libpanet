CC=`cat ../.compiler`
CCFLAGS=-I../src -fstrict-aliasing -O2 -Wall -Werror -pedantic
LDFLAGS=../src/libpanet.so -s
TARGET_BASIC_CLIENT_BIN=eg_basic_client
TARGET_BASIC_SERVER_BIN=eg_basic_server
TARGET_INFO_CLIENT_BIN=eg_info_client
TARGET_INFO_SERVER_BIN=eg_info_server
TARGET_UNIX_CLIENT_BIN=eg_unix_client
TARGET_UNIX_SERVER_BIN=eg_unix_server


all:
	${CC} ${CCFLAGS} -o ${TARGET_BASIC_CLIENT_BIN} eg_basic_client.c ${LDFLAGS}
	${CC} ${CCFLAGS} -o ${TARGET_BASIC_SERVER_BIN} eg_basic_server.c ${LDFLAGS}
	${CC} ${CCFLAGS} -o ${TARGET_INFO_SERVER_BIN} eg_info_server.c ${LDFLAGS}
	${CC} ${CCFLAGS} -o ${TARGET_UNIX_CLIENT_BIN} eg_unix_client.c ${LDFLAGS}
	${CC} ${CCFLAGS} -o ${TARGET_UNIX_SERVER_BIN} eg_unix_server.c ${LDFLAGS}


clean:
	rm -f ${TARGET_BASIC_CLIENT_BIN}
	rm -f ${TARGET_BASIC_SERVER_BIN}
	rm -f ${TARGET_INFO_SERVER_BIN}
	rm -f ${TARGET_UNIX_CLIENT_BIN}
	rm -f ${TARGET_UNIX_SERVER_BIN}

