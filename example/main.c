#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <signal.h>
#include <time.h>
// #include <glib.h>

#include "test_list.c"
#include "test_queue.c"
#include "test_cycbuf.c"
#include "test_string.c"
#include "test_rbtree.c"

typedef struct
{
    int     exit;
} test_context_t;

test_context_t g_test_ctx;

static void app_int_sig_handler(int sig)
{
    g_test_ctx.exit = 1;

    printf("app_int_sig_handler exit\n");
    // exit(0);
}

int main(int argc, const char *argv[])
{
    //int ret;

    printf("get user put cmdInfo:argc=%d\r\n", argc);

    for (int i = 0; i < argc; i++) {
        printf("%s ", argv[i]);
    }

    printf("\n");

    /* Register the signal handler. */
    signal(SIGINT, app_int_sig_handler);
    
    memset(&g_test_ctx, 0, sizeof(test_context_t));

    test_list();
    test_cycbuf();
    test_string();
    test_queue();
    test_rbtree();

    // while (!g_test_ctx.exit) {
    //     sleep(100);
    // }

    printf("demo leave \n");

    return 0;
}
