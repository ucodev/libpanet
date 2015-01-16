/**
 * @file timeout.c
 * @brief Portable Abstracted Network Library (libpanet)
 *        PANET Library Timeout Interface
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
#include <errno.h>

#include "config.h"
#include "panet.h"

#ifdef COMPILE_WIN32
DLLIMPORT
#endif
int panet_timeout_set(sock_t fd, int direction, struct timeval *timeo) {
	struct timeval tvnew = *timeo;
	socklen_t tv_size = sizeof(struct timeval);
	int optname = 0;

	if (!timeo)
		return 0;

	switch (direction) {
		case PANET_RECV: optname = SO_RCVTIMEO; break;
		case PANET_SEND: optname = SO_SNDTIMEO; break;
		default: errno = EINVAL; return -1;
	}

	if (getsockopt(fd, SOL_SOCKET, optname, (void *) timeo, &tv_size) < 0) {
		*timeo = tvnew;
		return -1;
	}

	if (setsockopt(fd, SOL_SOCKET, optname, (void *) &tvnew, sizeof(struct timeval)) < 0) {
		*timeo = tvnew;
		return -1;
	}

	return 0;
}

