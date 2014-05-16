/**
 * @file server.c
 * @brief Portable Abstracted Network Library (libpanet)
 *        PANET Library Server Interface
 *
 * Date: 05-10-2012
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
#include <errno.h>

#include "panet.h"


sock_t panet_server_ipv4(
		const char *host,
		const char *service,
		int proto,
		int backlog)
{
	sock_t fd;
	int socktype = 0;

	switch (proto) {
		case PANET_PROTO_TCP: socktype = SOCK_STREAM; break;
		case PANET_PROTO_UDP: socktype = SOCK_DGRAM; break;
		default: errno = EINVAL; return -1;
	}


	if ((fd = panet_bind(host, service, AF_INET, socktype)) < 0)
		return -1;

	if (proto == PANET_PROTO_TCP) {
		if (panet_listen(fd, backlog) < 0)
			return -1;
	}

	return fd;
}

sock_t panet_server_ipv6(
		const char *host,
		const char *service,
		int proto,
		int backlog)
{
	sock_t fd;
	int socktype = 0;

	switch (proto) {
		case PANET_PROTO_TCP: socktype = SOCK_STREAM; break;
		case PANET_PROTO_UDP: socktype = SOCK_DGRAM; break;
		default: errno = EINVAL; return -1;
	}

	if ((fd = panet_bind(host, service, AF_INET6, socktype)) < 0)
		return -1;

	if (proto == PANET_PROTO_TCP) {
		if (panet_listen(fd, backlog) < 0)
			return -1;
	}

	return fd;
}

sock_t panet_server(
		const char *host,
		const char *service,
		int proto,
		int backlog)
{
	sock_t fd;
	int socktype = 0;

	switch (proto) {
		case PANET_PROTO_TCP: socktype = SOCK_STREAM; break;
		case PANET_PROTO_UDP: socktype = SOCK_DGRAM; break;
		default: errno = EINVAL; return -1;
	}

	if ((fd = panet_bind(host, service, AF_UNSPEC, socktype)) < 0)
		return -1;

	if (proto == PANET_PROTO_TCP) {
		if (panet_listen(fd, backlog) < 0)
			return -1;
	}

	return fd;
}

