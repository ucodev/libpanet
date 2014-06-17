/**
 * @file panet.h
 * @brief Portable Abstracted Network Library (libpanet)
 *        PANET Library Interface Header
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

#ifndef LIBPANET_H
#define LIBPANET_H

 #if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined (_WIN64)
  #include <windows.h>
  #include <winsock2.h>
  #include <Ws2tcpip.h>
  #include <io.h>

  typedef SOCKET sock_t;

  #define panet_close(fd) _close(fd)
 #else
  #include <unistd.h>
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <sys/un.h>
  #include <netdb.h>
  #include <arpa/inet.h>

  typedef int sock_t;

  #define panet_close(fd) close(fd)
 #endif

enum {
	PANET_PROTO_TCP = 1,
	PANET_PROTO_UDP,
	PANET_PROTO_UNIX_STREAM,
	PANET_PROTO_UNIX_DGRAM
};

enum {
	PANET_SEND = 1,
	PANET_RECV
};

static inline int panet_eai_to_errno(int eia_err) {
	switch (eia_err) {
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined (_WIN64)
 #ifdef EAI_ADDRFAMILY
		case EAI_ADDRFAMILY: return EADDRNOTAVAIL;
 #endif
		case EAI_AGAIN: return EAGAIN;
		case EAI_BADFLAGS: return EINVAL;
		case EAI_FAIL: return EADDRNOTAVAIL;
		case EAI_FAMILY: return EAFNOSUPPORT;
		case EAI_MEMORY: return ENOMEM;
 #ifdef EAI_NODATA
		case EAI_NODATA: return EADDRNOTAVAIL;
 #endif
		case EAI_NONAME: return EINVAL;
		case EAI_SERVICE: return EINVAL;
		case EAI_SOCKTYPE: return ESOCKTNOSUPPORT;
		case EAI_SYSTEM:
#endif
		default: return errno;
	}

	return 0;
}


/* Prototypes / Interface */

/**
 * @brief
 *   Closes a socket identified by file descriptor 'fd'.
 *
 * @param fd
 *   Socket file descriptor.
 *
 * @return
 *   On success, zero is returned. On error, -1 is returned and errno is set
 *   appropriately.
 *
 */
int panet_safe_close(sock_t fd);

sock_t panet_bind(
		const char *host,
		const char *service,
		const char *path,
		int sockfamily,
		int socktype);
int panet_listen(sock_t fd, int backlog);
sock_t panet_connect(
		const char *host,
		const char *service,
		const char *path,
		long timeout,
		int sockfamily,
		int socktype);

/**
 * @brief
 *   Initiate a UNIX socket connection to named socket 'path' with 'proto'
 *   defining its behaviour: For streaming PANET_PROTO_UNIX_STREAM should
 *   be used, otherwise PANET_PROTO_UNIX_DGRAM is available for datagram
 *   based socket communications.
 *
 * @param path
 *   Path of the target named socket.
 *
 * @param proto
 *   Protocol: PANET_PROTO_UNIX_STREAM or PANET_PROTO_UNIX_DGRAM
 *
 * @return
 *   Returns a file descriptor on success. On error, -1 is returned and errno
 *   is set appropriately.
 *
 * @see panet_client_ipv4()
 * @see panet_client_ipv6()
 * @see panet_safe_close()
 *
 */
sock_t panet_client_unix(
		const char *path,
		int proto);

/**
 * @brief
 *   Initiate a IPv4 connection to 'service' on 'host' with 'proto' as its OSI
 *   layer 4 protocol.
 *
 * @param host
 *   Remote hostname or IPv4 address. If this value is NULL, localhost will be
 *   used.
 *
 * @param service
 *   Remote service name or numeric service port.
 *
 * @param proto
 *   Protocol: PANET_PROTO_TCP or PANET_PROTO_UDP
 *
 * @param timeout
 *   Time to wait before give up on connect(). The value is in seconds. This
 *   value is ignored when PANET_PROTO_UDP is set on 'proto'.
 *
 * @return
 *   Returns a file descriptor on success. On error, -1 is returned and errno
 *   is set appropriately.
 *
 * @see panet_client()
 * @see panet_client_ipv6()
 * @see panet_safe_close()
 *
 */
sock_t panet_client_ipv4(
		const char *host,
		const char *service,
		int proto,
		long timeout);

/**
 * @brief
 *   Initiate a IPv6 connection to 'service' on 'host' with 'proto' as its OSI
 *   layer 4 protocol.
 *
 * @param host
 *   Remote hostname or IPv6 address. If this value is NULL, localhost will be
 *   used.
 *
 * @param service
 *   Remote service name or numeric service port.
 *
 * @param proto
 *   Protocol: PANET_PROTO_TCP or PANET_PROTO_UDP
 *
 * @param timeout
 *   Time to wait before give up on connect(). The value is in seconds. This
 *   value is ignored when PANET_PROTO_UDP is set on 'proto'.
 *
 * @return
 *   Returns a file descriptor on success. On error, -1 is returned and errno
 *   is set appropriately.
 *
 * @see panet_client()
 * @see panet_client_ipv4()
 * @see panet_safe_close()
 *
 */
sock_t panet_client_ipv6(
		const char *host,
		const char *service,
		int proto,
		long timeout);

/**
 * @brief
 *   Initiate a connection to 'service' on 'host' with 'proto' as its OSI layer
 *   4 protocol. OSI layer 3 is automatically detected based on 'host' value.
 *
 * @param host
 *   Remote hostname or IPv4 or IPv6 address. If this value is NULL, localhost
 *   will be used.
 *
 * @param service
 *   Remote service name or numeric service port.
 *
 * @param proto
 *   Protocol: PANET_PROTO_TCP or PANET_PROTO_UDP
 *
 * @param timeout
 *   Time to wait before give up on connect(). The value is in seconds. This
 *   value is ignored when PANET_PROTO_UDP is set on 'proto'.
 *
 * @return
 *   Returns a file descriptor on success. On error, -1 is returned and errno
 *   is set appropriately.
 *
 * @see panet_client()
 * @see panet_client_ipv4()
 * @see panet_client_ipv6()
 * @see panet_safe_close()
 *
 */
sock_t panet_client(
		const char *host,
		const char *service,
		int proto,
		long timeout);

/**
 * @brief
 *   Initiate a UNIX named socket 'path' with 'proto' defining its behaviour:
 *   For streaming PANET_PROTO_UNIX_STREAM should be used, otherwise
 *   PANET_PROTO_UNIX_DGRAM is available for datagram based socket
 *   communications.
 *
 * @param path
 *   Path of the target named socket.
 *
 * @param proto
 *   Protocol: PANET_PROTO_UNIX_STREAM or PANET_PROTO_UNIX_DGRAM
 * 
 * @param backlog
 *   listen() backlog parameter. This value is ignored when PANET_PROTO_UDP
 *   value is used on 'proto' parameter.
 
 * @return
 *   Returns a file descriptor on success. On error, -1 is returned and errno
 *   is set appropriately.
 *
 * @see panet_server()
 * @see panet_server_ipv4()
 * @see panet_server_ipv6()
 * @see panet_safe_close()
 *
 */
sock_t panet_server_unix(
		const char *path,
		int proto,
		int backlog);

/**
 * @brief
 *   Initiate a 'service' on 'host' with 'proto' as its OSI layer 4 protocol.
 *
 * @param host
 *   An IPv4 address configured on an network interface of the system executing
 *   this call. This value can also be a hostname value that resolves to a
 *   local configured IPv4 address.
 *
 * @param service
 *   A service name or numeric service port available for bind()ing on the
 *   system executing this call.
 *
 * @param proto
 *   Protocol: PANET_PROTO_TCP or PANET_PROTO_UDP
 *
 * @param backlog
 *   listen() backlog parameter. This value is ignored when PANET_PROTO_UDP
 *   value is used on 'proto' parameter.
 *
 * @return
 *   Returns a file descriptor on success. On error, -1 is returned and errno
 *   is set appropriately.
 *
 * @see panet_server()
 * @see panet_server_ipv6()
 * @see panet_safe_close()
 *
 */
sock_t panet_server_ipv4(
		const char *host,
		const char *service,
		int proto,
		int backlog);

/**
 * @brief
 *   Initiate a 'service' on 'host' with 'proto' as its OSI layer 4 protocol.
 *
 * @param host
 *   An IPv6 address configured on an network interface of the system executing
 *   this call. This value can also be a hostname value that resolves to a
 *   local configured IPv6 address.
 *
 * @param service
 *   A service name or numeric service port available for bind()ing on the
 *   system executing this call.
 *
 * @param proto
 *   Protocol: PANET_PROTO_TCP or PANET_PROTO_UDP
 *
 * @param backlog
 *   listen() backlog parameter. This value is ignored when PANET_PROTO_UDP
 *   value is used on 'proto' parameter.
 *
 * @return
 *   Returns a file descriptor on success. On error, -1 is returned and errno
 *   is set appropriately.
 *
 * @see panet_server()
 * @see panet_server_ipv4()
 * @see panet_safe_close()
 *
 */
sock_t panet_server_ipv6(
		const char *host,
		const char *service,
		int proto,
		int backlog);

/**
 * @brief
 *   Initiate a 'service' on 'host' with 'proto' as its OSI layer 4 protocol.
 *
 * @param host
 *   An IPv4 or IPv6 address configured on an network interface of the system
 *   executing this call. This value can also be a hostname value that resolves
 *   to a local configured IPv4 or IPv6 address.
 *
 * @param service
 *   A service name or numeric service port available for bind()ing on the
 *   system executing this call.
 *
 * @param proto
 *   Protocol: PANET_PROTO_TCP or PANET_PROTO_UDP
 *
 * @param backlog
 *   listen() backlog parameter. This value is ignored when PANET_PROTO_UDP
 *   value is used on 'proto' parameter.
 *
 * @return
 *   Returns a file descriptor on success. On error, -1 is returned and errno
 *   is set appropriately.
 * 
 * @see panet_server()
 * @see panet_server_ipv4()
 * @see panet_server_ipv6()
 * @see panet_safe_close()
 *
 */
sock_t panet_server(
		const char *host,
		const char *service,
		int proto,
		int backlog);

/**
 * @brief
 *   Translates the address and service pointed by 'addr' with length 'alen' to
 *   hostname 'host' and service name 'service'.
 *
 * @param addr
 *   Socket address structure.
 *
 * @param alen
 *   Socket address structure length.
 *
 * @param host
 *   The translated address to name. If NULL, no host translation is performed.
 *
 * @param service
 *   The translated port to service name. If NULL, no name translation is
 *   performed.
 *
 * @param flags
 *   The flags field that will be passed to getnameinfo() function.
 *
 * @return
 *   Returns zero on success. On error, -1 is returned and errno is set
 *   appropriately. The 'host' and 'service', if not NULL, are automatically
 *   allocated through malloc() (or fsma_malloc() if libfsma is used).
 *
 * @see panet_info_sock()
 * @see panet_info_free()
 *
 */
int panet_info_addr(
		struct sockaddr *addr,
		socklen_t alen,
		char **host,
		char **service,
		int flags);

/**
 * @brief
 *   Translates the address and service described by file descriptor 'fd' to
 *   hostname 'host' and service name 'service'.
 *
 * @param fd
 *   Socket file descriptor.
 *
 * @param host
 *   The translated address to name. If NULL, no host translation is performed.
 *
 * @param service
 *   The translated port to service name. If NULL, no name translation is
 *   performed.
 *
 * @param flags
 *   The flags field that will be passed to getnameinfo() function.
 *
 * @return
 *   Returns zero on success. On error, -1 is returned and errno is set
 *   appropriately. The 'host' and 'service', if not NULL, are automatically
 *   allocated through malloc() (or fsma_malloc() if libfsma is used).
 *
 * @see panet_info_addr()
 * @see panet_info_free()
 *
 */
int panet_info_sock(sock_t fd, char **host, char **service, int flags);

/**
 * @brief
 *   Releases, through free() (or fsma_free() if libfsma is used), the memory
 *   allocated to 'host' and 'service', previously allocated by
 *   panet_info_addr() or panet_info_sock().
 *
 * @param host
 *   Host pointer passed to panet_info_addr() or panet_info_sock().
 *
 * @param service
 *   Service pointer passed to panet_info_addr() or panet_info_sock().
 *
 * @see panet_info_addr()
 * @see panet_info_sock()
 *
 */
void panet_info_free(char *host, char *service);

/**
 * @brief
 *   Sets the receiving or sending timeouts for a particular socket identified
 *   by file descriptor 'fd'. This is an abstraction for SO_RCVTIMEO and
 *   SO_SNDTIMEO optname values for setsockopt().
 *
 * @param fd
 *   Socket file descriptor.
 *
 * @param direction
 *   PANET_RECV to affect receiving, or PANET_SEND to affect sending.
 *
 * @param timeo
 *   The timeout value to be set.
 *
 * @return
 *   On success, zero is returned and 'timeo' parameter is set to the previously
 *   value set. On error, -1 is returned, 'timeo' isn't changed, and errno is
 *   set appropriately.
 *
 * @see panet_client()
 * @see panet_client_ipv4()
 * @see panet_client_ipv6()
 *
 */
int panet_timeout_set(sock_t fd, int direction, struct timeval *timeo);

#endif

