#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "panet.h"

int main(void) {
	int fd, fd_acpt;
	char data[14];

	memset(data, 0, 14);

	if ((fd = panet_server_unix("/tmp/panet.sock", PANET_PROTO_UNIX_STREAM, 5)) < 0) {
		fprintf(stderr, "panet_server() error: %s\n", strerror(errno));
		return 1;
	}

	if ((fd_acpt = accept(fd, NULL, NULL)) < 0) {
		fprintf(stderr, "accept() error: %s\n", strerror(errno));
		panet_safe_close(fd);
		return 1;
	}

	if (read(fd_acpt, data, 14) != 14) {
		fprintf(stderr, "read() error: %s\n", strerror(errno));
		panet_safe_close(fd_acpt);
		panet_safe_close(fd);
		return 1;
	}

	if (write(fd_acpt, "Hello Client!", 14) != 14) {
		fprintf(stderr, "write() error: %s\n", strerror(errno));
		panet_safe_close(fd_acpt);
		panet_safe_close(fd);
		return 1;
	}

	printf("Received: '%s' from client.\n", data);

	panet_safe_close(fd_acpt);
	panet_safe_close(fd);

	return 0;
}

