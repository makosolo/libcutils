#ifndef H_UTILS_NET_SOCK_H
#define H_UTILS_NET_SOCK_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

#define UTIL_NET_PRINT(msg, ...)         printf("[%s:%d] "msg, __FUNCTION__, __LINE__, ##__VA_ARGS__)

/* Maximum Simultaneous client connections. */
#define UTIL_NET_MAX_LISTEN_CNT            5

/* Maximum allowed payload size - 1 MB */
#define UTIL_NET_MAX_PAYLOAD_SIZE        (1024 * 1024)

/* Maximum transfer size - 256 KB */
#define UTIL_NET_MAX_XFER_SIZE           (256 * 1024)

#define UTIL_NET_MAX_NUM_FD              128

#define UTIL_NET_RET_PENDING             1
#define UTIL_NET_RET_SUCCESS             0
#define UTIL_NET_RET_FAILURE             (-1)
#define UTIL_NET_SOCK_CREATE_ERROR       (UTIL_NET_RET_FAILURE-2)
#define UTIL_NET_SOCK_BIND_ERROR         (UTIL_NET_RET_FAILURE-3)
#define UTIL_NET_SOCK_LISTEN_ERROR       (UTIL_NET_RET_FAILURE-4)
#define UTIL_NET_SOCK_CONNECT_ERROR      (UTIL_NET_RET_FAILURE-5)
#define UTIL_NET_SOCK_RCV_ERROR          (UTIL_NET_RET_FAILURE-6)
#define UTIL_NET_INVALID_PAYLOAD_SIZE    (UTIL_NET_RET_FAILURE-7)
#define UTIL_NET_INVALID_HDR_INFO        (UTIL_NET_RET_FAILURE-7)
#define UTIL_NET_SOCK_SEND_ERROR         (UTIL_NET_RET_FAILURE-9)
#define UTIL_NET_SOCK_FCNTL_ERROR        (UTIL_NET_RET_FAILURE-10)

typedef void (*OnUtilNetClientProcess)(void *ctx, void *user_data);

typedef struct
{
    /** Server state. */
    uint8_t                     state;

    /** Flag to control the server exit. */
    uint8_t                     exit_flag;

    /** Server socket Id. */
    int32_t                     svr_sock;

    /** Generic data type to carry port/pathname. */
    char                       *path_name;

    /** Server Processing function. */
    OnUtilNetClientProcess      client_process;

    /** Number of concurrent clients to handle. */
    uint32_t                    listen_cnt;

    /** Application specific auxiliary data. */
    void                       *user_data;

} util_net_server_context_t;

/* server interface */
int32_t util_net_create_server(util_net_server_context_t *ctx);
int32_t util_net_destroy_server(util_net_server_context_t *ctx);
int32_t util_net_process_server(util_net_server_context_t *ctx);

/* client interface */
int32_t util_net_connect_client(const char  *path_name);

/* common interface */
int32_t util_net_write_msg(int32_t sock_fd, void *buff, int32_t size, int32_t fd[], uint32_t num);
int32_t util_net_read_msg(int32_t sock_fd, void *buff, uint32_t size, int32_t fd[], uint32_t *num);
int32_t util_net_write_data(int32_t sock_fd, void *buff, int32_t size);
int32_t util_net_read_data(int32_t sock_fd, void *buff, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif /* H_UTILS_NET_SOCK_H */

