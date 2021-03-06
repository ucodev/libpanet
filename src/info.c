/**
 * @file info.c
 * @brief Portable Abstracted Network Library (libpanet)
 *        PANET Library Info Interface
 *
 * Date: 16-01-2015
 * 
 * Copyright 2012-2015 Pedro A. Hortas (pah@ucodev.org)
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
#include <stdlib.h>
#include <errno.h>

#include "config.h"
#include "mm.h"
#include "panet.h"

#ifdef COMPILE_WIN32
DLLIMPORT
#endif
int panet_info_addr(
		struct sockaddr *addr,
		socklen_t alen,
		char **host,
		char **service,
		int flags)
{
	int s = 0, errsv = 0;
	char thost[NI_MAXHOST], tservice[NI_MAXSERV];

	if ((s = getnameinfo(addr, alen, host ? thost : NULL, sizeof(thost), service ? tservice : NULL, sizeof(tservice), flags)) < 0) {
		errno = panet_eai_to_errno(s);
		return -1;
	}

	if (host) {
		if (!(*host = (char *) mm_alloc(strlen(thost) + 1)))
			return -1;

		memcpy(*host, thost, strlen(thost) + 1);
	}

	if (service) {
		if (!(*service = (char *) mm_alloc(strlen(tservice) + 1))) {
			errsv = errno;
			if (host) mm_free(*host);
			errno = errsv;
			return -1;
		}

		memcpy(*service, tservice, strlen(tservice) + 1);
	}

	return 0;

}

#ifdef COMPILE_WIN32
DLLIMPORT
#endif
int panet_info_sock_addr(sock_t fd, char **host, char **service, int flags) {
	struct sockaddr_storage addr;
	socklen_t alen = sizeof(struct sockaddr_storage);

	if (getsockname(fd, (struct sockaddr *) &addr, &alen) < 0)
		return -1;

	return panet_info_addr((struct sockaddr *) &addr, alen, host, service, flags);
}

void panet_info_addr_free(char *host, char *service) {
	if (host)
		mm_free(host);

	if (service)
		mm_free(service);
}

#ifdef COMPILE_WIN32
DLLIMPORT
#endif
int panet_info_sock_family(sock_t fd) {
	struct sockaddr_storage addr;
	socklen_t alen = sizeof(struct sockaddr_storage);

	if (getsockname(fd, (struct sockaddr *) &addr, &alen) < 0)
		return -1;

	return ((struct sockaddr *) &addr)->sa_family;
}

