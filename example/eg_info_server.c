#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "panet.h"

int main(void) {
	int fd, fd_acpt;
	char data[14];
	struct sockaddr_storage addr;
	socklen_t alen = sizeof(struct sockaddr_storage);
	char *host = NULL, *service = NULL;

	memset(data, 0, 14);

	if ((fd = panet_server("localhost", "9876", PANET_PROTO_TCP, 10)) < 0) {
		fprintf(stderr, "panet_server() error: %s\n", strerror(errno));
		return 1;
	}

	if ((fd_acpt = accept(fd, (struct sockaddr *) &addr, &alen)) < 0) {
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

	if (panet_info_addr((struct sockaddr *) &addr, alen, &host, &service, NI_NUMERICHOST | NI_NUMERICSERV) < 0) {
		fprintf(stderr, "panet_info_addr() error: %s\n", strerror(errno));
		panet_safe_close(fd_acpt);
		panet_safe_close(fd);
		return 1;
	}

	printf("Received: '%s' from client (address: '%s', port: '%s').\n", data, host, service);

	panet_info_free(host, service);

	panet_safe_close(fd_acpt);
	panet_safe_close(fd);

	return 0;
}

