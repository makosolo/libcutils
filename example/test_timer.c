#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#include "utils/utils_assert.h"
#include "utils/utils_timer.h"

typedef struct
{
    int        exit;
} timer_context_t;

static timer_context_t g_timer_context_t;

static uint64_t PlatformTicksGetFunc(void)
{
    struct timespec current_time;
    clock_gettime(CLOCK_MONOTONIC, &current_time);
    return (uint64_t)((current_time.tv_sec * 1000) + (current_time.tv_nsec / 1000000));
}

static void onTimer_1s(void *arg)
{
    printf("onTimer_1s, tick=%ld \n", PlatformTicksGetFunc());
}

static void onTimer_2s(void *arg)
{
    printf("onTimer_2s, tick=%ld \n", PlatformTicksGetFunc());
}

static void onTimer_3s(void *arg)
{
    printf("onTimer_3s, tick=%ld \n", PlatformTicksGetFunc());
}

static void app_int_sig_handler(int sig)
{
    g_timer_context_t.exit = 1;

    printf("app_int_sig_handler exit\n");
    // exit(0);
}

void test_timer(void)
{
    util_timer_t timer;
    uint64_t deadline = 0;

    printf("++++++++++++++++++++++++timer++++++++++++++++++++++++\n");

    UTIL_ASSERT_RET(0 == util_timer_create(&timer, PlatformTicksGetFunc), "util_timer_create fail");

    UTIL_ASSERT_RET(0 == util_timer_start(&timer, onTimer_1s, NULL, 0, 1000, true), "util_timer_start fail");
    UTIL_ASSERT_RET(0 == util_timer_start(&timer, onTimer_2s, NULL, 0, 2000, true), "util_timer_start fail");
    UTIL_ASSERT_RET(0 == util_timer_start(&timer, onTimer_3s, NULL, 0, 3000, true), "util_timer_start fail");

    while (!g_timer_context_t.exit) {
        deadline = util_timer_timeout(&timer);
        if (0 != deadline) {
            // printf("util_timer_timeout=%ld \n", deadline);
        }

        deadline /= 10;
        if (deadline > 0) {
            usleep(deadline * 1000);
        }
        else {
            usleep(1000);
        }
    }

}
