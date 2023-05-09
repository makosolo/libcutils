#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <signal.h>
#include <time.h>
// #include <glib.h>

#include "remote_debug_server.h"

typedef struct
{
    uint8_t borad;
    int     exit;
} test_context_t;

test_context_t g_test_ctx;

static void app_int_sig_handler(int sig)
{
    g_test_ctx.exit = 1;

    printf("app_int_sig_handler exit\n");
    // exit(0);
}

int OnCmdDiscoverProcessCallback(int argc, char *argv[], ft_remote_debug_response_t *response)
{
    printf("OnCmdDiscoverProcessCallback: argc=%d \n", argc);
    for (int i = 0; i < argc; i++) {
        printf("OnCmdDiscoverProcessCallback: argv[%d]=%s \n", i, argv[i]);
    }

    response->result = 0;
    strncpy(response->buffer, "123456", FT_REMOTE_DEBUG_SERVER_BUFFER_SIZE);

    return 0;
}

int main(int argc, const char *argv[])
{
    //int ret;
    uint8_t level = 0, verbose = 0, buffer_recycle = 0, test_case = 0;

    printf("get user put cmdInfo:argc=%d\r\n", argc);

    for (int i = 0; i < argc; i++) {
        printf("%s ", argv[i]);
    }

    printf("\n");

    // if (argc < 2) {
    //     print_usage();
    //     return -1;
    // }

    /* Register the signal handler. */
    signal(SIGINT, app_int_sig_handler);
    
    memset(&g_test_ctx, 0, sizeof(test_context_t));

    int handle = ft_remote_debug_server_create("/tmp/ft_dsp_debug_server");
    printf("handle=%d \n", handle);
    if (handle > 0) {
        ft_remote_debug_server_register(handle, "cmd_discover", OnCmdDiscoverProcessCallback);

        while (!g_test_ctx.exit) {
            sleep(100);
        }

        ft_remote_debug_server_destroy(handle);
    }

    printf("demo leave \n");

    return 0;
}
