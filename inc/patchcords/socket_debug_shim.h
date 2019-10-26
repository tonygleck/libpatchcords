// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef SOCKET_DEBUG_SHIM_H
#define SOCKET_DEBUG_SHIM_H

#ifdef __cplusplus
#include <cstdlib>
#include <cstdint>
extern "C" {
#else
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#endif

#include "umock_c/umock_c_prod.h"
#ifdef WIN32
    #include <winsock2.h>
#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netdb.h>
#endif

#if defined(USE_SOCKET_DEBUG_SHIM)
MOCKABLE_FUNCTION(, int, socket_shim_init);
MOCKABLE_FUNCTION(, void, socket_shim_deinit);

#ifdef WIN32
MOCKABLE_FUNCTION(, SOCKET, socket_shim_socket, int, domain, int, type, int, protocol);
#else
MOCKABLE_FUNCTION(, int, socket_shim_socket, int, domain, int, type, int, protocol);
#endif

#ifdef WIN32
MOCKABLE_FUNCTION(, int, socket_shim_send, SOCKET, sock, const char*, buf, int, len, int, flags);
#else
MOCKABLE_FUNCTION(, ssize_t, socket_shim_send, int, sock, const void*, buf, size_t, len, int, flags);
#endif

#ifdef WIN32
MOCKABLE_FUNCTION(, int, socket_shim_recv, SOCKET, sock, char*, buf, int, len, int, flags);
#else
MOCKABLE_FUNCTION(, ssize_t, socket_shim_recv, int, sock, void*, buf, size_t, len, int, flags);
#endif

#ifdef WIN32
MOCKABLE_FUNCTION(, int, socket_shim_connect, SOCKET, sock, const sockaddr*, name, int, len);
#else
MOCKABLE_FUNCTION(, int, socket_shim_connect, int, sock, __CONST_SOCKADDR_ARG, addr, socklen_t, len);
#endif

#ifdef WIN32
MOCKABLE_FUNCTION(, int, socket_shim_getaddrinfo, PCSTR, node, PCSTR, svc_name, const ADDRINFOA*, hints, PADDRINFOA*, res);
#else
MOCKABLE_FUNCTION(, int, socket_shim_getaddrinfo, const char*, node, const char*, svc_name, const struct addrinfo*, hints, struct addrinfo**, res);
#endif

#ifdef WIN32
MOCKABLE_FUNCTION(, int, socket_shim_shutdown, int, node, int, how);
#else
MOCKABLE_FUNCTION(, int, socket_shim_shutdown, int, sockfd, int, how);
#endif

#ifdef WIN32
MOCKABLE_FUNCTION(, int, socket_shim_close, int, sock);
#else
MOCKABLE_FUNCTION(, int, socket_shim_close, int, sock);
#endif

#ifdef WIN32
MOCKABLE_FUNCTION(, void, socket_shim_freeaddrinfo, struct addrinfo*, res);
#else
MOCKABLE_FUNCTION(, void, socket_shim_freeaddrinfo, struct addrinfo*, res);
#endif

extern int socket_shim_fcntl(int __fd, int __cmd, ...);

MOCKABLE_FUNCTION(, uint64_t, socket_shim_get_bytes_sent);
MOCKABLE_FUNCTION(, uint64_t, socket_shim_get_num_sends);
MOCKABLE_FUNCTION(, uint64_t, socket_shim_get_bytes_recv);
MOCKABLE_FUNCTION(, uint64_t, socket_shim_get_num_recv);
MOCKABLE_FUNCTION(, void, socket_shim_reset);

#define socket socket_shim_socket
#define send socket_shim_send
#define recv socket_shim_recv
#define connect socket_shim_connect
#define fcntl socket_shim_fcntl
#define close socket_shim_close
#define shutdown socket_shim_shutdown
#define getaddrinfo socket_shim_getaddrinfo
#define freeaddrinfo socket_shim_freeaddrinfo

#else // USE_SOCKET_DEBUG_SHIM

#define socket_shim_init() 0
#define socket_shim_deinit() ((void)0)

#define socket_shim_get_bytes_sent     0
#define socket_shim_get_num_sends      0
#define socket_shim_get_bytes_recv     0
#define socket_shim_get_num_recv       0
#define socket_shim_reset()            0

#endif  // USE_SOCKET_DEBUG_SHIM

#ifdef __cplusplus
}
#endif

#endif // SOCKET_DEBUG_SHIM_H