/**
 * @file low.c
 * @brief Portable Abstracted Network Library (libpanet)
 *        PANET Library Low Level Interface
 *
 * Date: 13-10-2012
 * 
 * Copyright 2012 Pedro A. Hortas (pah@ucodev.org)
 *
 * This file is part of libpanet.
 *
 * libpanet is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libpanet is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libpanet.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "panet.h"


int panet_safe_close(sock_t fd) {
	int ret = 0;

	while ((ret = panet_close(fd)) < 0) {
		if (errno != EINTR)
			break;
	}

	return ret;
}

sock_t panet_bind(
		const char *host,
		const char *service,
		int sockfamily,
		int socktype)
{
	sock_t fd = 0;
	int s = 0, optval = 1;
	struct addrinfo hints, *rcur = NULL, *rlist = NULL;

	memset(&hints, 0, sizeof(struct addrinfo));

	hints.ai_family = sockfamily;
	hints.ai_socktype = socktype;
	hints.ai_flags = AI_PASSIVE;

	if ((s = getaddrinfo(host, service, &hints, &rlist))) {
		errno = panet_eai_to_errno(s);
		return -1;
	}

	for (rcur = rlist; rcur; rcur = rcur->ai_next) {
		if ((fd = socket(rcur->ai_family, rcur->ai_socktype, rcur->ai_protocol)) < 0)
			continue;

		if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (void *) &optval, sizeof(optval)) < 0) {
			panet_safe_close(fd);
			continue;
		}

		if (bind(fd, rcur->ai_addr, rcur->ai_addrlen) < 0) {
			panet_safe_close(fd);
			continue;
		}

		break;
	}

	freeaddrinfo(rlist);

	if (!rcur)
		return -1;

	return fd;
}

int panet_listen(sock_t fd, int backlog) {
	int errsv = 0;

	if (listen(fd, backlog) < 0) {
		errsv = errno;
		panet_safe_close(fd);
		errno = errsv;
		return -1;
	}

	return 0;
}

sock_t panet_connect(
		const char *host,
		const char *service,
		long timeout,
		int sockfamily,
		int socktype)
{
	sock_t fd = 0;
	int s = 0, errsv = 0;
	struct timeval tvto;
	struct addrinfo hints, *rcur = NULL, *rlist = NULL;

	memset(&hints, 0, sizeof(struct addrinfo));

	hints.ai_family = sockfamily;
	hints.ai_socktype = socktype;

	if ((s = getaddrinfo(host, service, &hints, &rlist))) {
		errno = panet_eai_to_errno(s);
		return -1;
	}

	for (rcur = rlist; rcur; rcur = rcur->ai_next) {
		if ((fd = socket(rcur->ai_family, rcur->ai_socktype, rcur->ai_protocol)) < 0)
			continue;

		if (timeout > 0) {
			tvto.tv_sec = timeout;
			tvto.tv_usec = 0;

			if (panet_timeout_set(fd, PANET_RECV, &tvto) < 0) {
				panet_safe_close(fd);
				continue;
			}
		}

		if (connect(fd, rcur->ai_addr, rcur->ai_addrlen) < 0) {
			panet_safe_close(fd);
			continue;
		}

		break;
	}

	freeaddrinfo(rlist);

	if (!rcur)
		return -1;

	if (panet_timeout_set(fd, PANET_RECV, &tvto) < 0) {
		errsv = errno;
		panet_safe_close(fd);
		errno = errsv;
		return -1;
	}

	return fd;
}

