#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "panet.h"

int main(void) {
	int fd;
	char data[14];

	memset(data, 0, 14);

	if ((fd = panet_client("localhost", "9876", PANET_PROTO_TCP, 5)) < 0) {
		fprintf(stderr, "panet_client() error: %s\n", strerror(errno));
		return 1;
	}

	if (write(fd, "Hello Server!", 14) != 14) {
		fprintf(stderr, "write() error: %s\n", strerror(errno));
		panet_safe_close(fd);
		return 1;
	}

	if (read(fd, data, 14) != 14) {
		fprintf(stderr, "read() error: %s\n", strerror(errno));
		panet_safe_close(fd);
		return 1;
	}

	printf("Received: '%s' from server.\n", data);

	panet_safe_close(fd);

	return 0;
}

