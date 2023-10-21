#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <time.h>

#include "platform/utils_task.h"
#include "utils/utils_counter.h"

typedef struct {
    util_task_t*    task;
    uint32_t		stop;
    uint32_t		stop_done;
} task_obj_t;

typedef struct
{
    util_counter_t *counter;
    task_obj_t counter_task;;
} counter_context_t;

counter_context_t g_counter_ctx;

static void app_counter_task(void *app_var)
{
    counter_context_t *pObj = (counter_context_t*)app_var;
    if (NULL == pObj) {
        return;
    }

    uint32_t count = time((time_t*)NULL);

    util_counter_start(pObj->counter, count, 3);
    printf("util_counter_start: count=%d started=%d \n", count, util_counter_started(pObj->counter));

    while(!pObj->counter_task.stop) {
        count = time(0);

        util_counter_start(pObj->counter, count, 10);
        if (0 == util_counter_remain(pObj->counter, count, false)) {
            printf("+++util_counter_remain:count=%d started=%d\n", count, util_counter_started(pObj->counter));
            util_counter_start(pObj->counter, count, 10);
            printf("---util_counter_remain:count=%d started=%d\n", count, util_counter_started(pObj->counter));
            break;
        }
    }

    count = time(0);
    util_counter_restart(pObj->counter, count, 0);
    printf("util_counter_restart: count=%d started=%d \n", count, util_counter_started(pObj->counter));

    while(!pObj->counter_task.stop) {
        count = time(0);

        if (0 == util_counter_remain(pObj->counter, count, true)) {
            printf("util_counter_remain:count=%d started=%d\n", count, util_counter_started(pObj->counter));
            // break;
        }
    }
    util_counter_stop(pObj->counter);
    printf("util_counter_remain: started=%d\n", util_counter_started(pObj->counter));

    pObj->counter_task.stop_done = 1;
}

static void app_int_sig_handler(int sig)
{
    g_counter_ctx.counter_task.stop = 1;

    printf("app_int_sig_handler exit\n");
    // exit(0);
}

void test_counter(void)
{
    printf("++++++++++++++++++++++++counter++++++++++++++++++++++++\n");

    signal(SIGINT, app_int_sig_handler);
    
    util_counter_t *counter = util_counter_create(3);
    if (NULL == counter) {
        return;
    }

    util_task_create_params_t params;

    params.core_affinity = UTIL_TASK_AFFINITY_ANY;
    params.priority      = UTIL_TASK_PRI_HIGHEST-2;
    params.task_main     = app_counter_task;
    params.app_var       = &g_counter_ctx;
    strncpy(params.task_name, "VCounterTask", UTIL_MAX_TASK_NAME);
    params.task_name[UTIL_MAX_TASK_NAME-1U] = (char)0;

    int status = util_task_create(&g_counter_ctx.counter_task.task, &params);
    if (0 == status) {
        g_counter_ctx.counter_task.stop_done = 0;
        g_counter_ctx.counter_task.stop      = 0;
        g_counter_ctx.counter                = counter;

        if (NULL != g_counter_ctx.counter_task.task) {
            while (!g_counter_ctx.counter_task.stop_done) {
                util_task_wait_msecs(100);
            }

            util_task_delete(&g_counter_ctx.counter_task.task);
        }
    }


    util_counter_destroy(counter);
}
