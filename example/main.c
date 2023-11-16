#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <signal.h>
#include <time.h>

#include "utils_comm.h"

extern void test_counter(void);
extern void test_cycbuf(void);
extern void test_list(void);
extern void test_cycqueue(void);
extern void test_string(void);
extern void test_task(void);
extern void test_mutex(void);
extern void test_event(void);
extern void test_queue(void);
extern void test_log(void);
extern void test_map(void);
extern void test_array(void);
extern void test_heap(void);

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
    int mode = 0;

    printf("get user put cmdInfo:argc=%d\r\n", argc);

    for (int i = 0; i < argc; i++) {
        printf("%s ", argv[i]);
    }

    if (argc > 0) {
        mode = atoi(argv[1]);
    }
    else {

    }

    printf("\n");

    /* Register the signal handler. */
    signal(SIGINT, app_int_sig_handler);

    memset(&g_test_ctx, 0, sizeof(test_context_t));

    printf("version:%s \n", util_get_version());

    switch (mode) {
    case 0: test_list(); break;
    case 1: test_cycbuf(); break;
    case 2: test_string(); break;
    case 3: test_cycqueue(); break;
    case 4: test_counter(); break;
    case 5: test_task(); break;
    case 6: test_mutex(); break;
    case 7: test_event(); break;
    case 8: test_queue(); break;
    case 9: test_log(); break;
    case 10: test_map(); break;
    case 11: test_array(); break;
    case 12: test_heap(); break;
    default: break;
    }

    // while (!g_test_ctx.exit) {
    //     sleep(100);
    // }

    printf("demo leave \n");

    return 0;
}
