/**
 * @file client.c
 * @brief Portable Abstracted Network Library (libpanet)
 *        PANET Library Client Interface
 *
 * Date: 17-06-2014
 * 
 * Copyright 2012-2014 Pedro A. Hortas (pah@ucodev.org)
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

sock_t panet_client_unix(
		const char *path,
		int proto,
		long timeout)
{
	int socktype = 0;

	switch (proto) {
		case PANET_PROTO_UNIX_STREAM: socktype = SOCK_STREAM; break;
		case PANET_PROTO_UNIX_DGRAM: socktype = SOCK_DGRAM; break;
		default: errno = EINVAL; return -1;
	}

	return panet_connect(NULL, NULL, path, timeout, AF_UNIX, socktype);
}

sock_t panet_client_ipv4(
		const char *host,
		const char *service,
		int proto,
		long timeout)
{
	int socktype = 0;

	switch (proto) {
		case PANET_PROTO_TCP: socktype = SOCK_STREAM; break;
		case PANET_PROTO_UDP: socktype = SOCK_DGRAM; break;
		default: errno = EINVAL; return -1;
	}

	return panet_connect(host, service, NULL, timeout, AF_INET, socktype);
}

sock_t panet_client_ipv6(
		const char *host,
		const char *service,
		int proto,
		long timeout)
{
	int socktype = 0;

	switch (proto) {
		case PANET_PROTO_TCP: socktype = SOCK_STREAM; break;
		case PANET_PROTO_UDP: socktype = SOCK_DGRAM; break;
		default: errno = EINVAL; return -1;
	}

	return panet_connect(host, service, NULL, timeout, AF_INET6, socktype);
}

sock_t panet_client(
		const char *host,
		const char *service,
		int proto,
		long timeout)
{
	int socktype = 0;

	switch (proto) {
		case PANET_PROTO_TCP: socktype = SOCK_STREAM; break;
		case PANET_PROTO_UDP: socktype = SOCK_DGRAM; break;
		default: errno = EINVAL; return -1;
	}

	return panet_connect(host, service, NULL, timeout, AF_UNSPEC, socktype);
}


