#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <signal.h>

#include "platform/include/utils_task.h"
#include "platform/include/utils_event.h"

typedef struct
{
    util_task_t*    task;
    uint32_t		stop;
    uint32_t		stop_done;
} task_obj_t;

typedef struct
{
    task_obj_t task_1;
    task_obj_t task_2;
    int        exit;
    util_event_t *event;
} event_context_t;

event_context_t g_event_ctx;

static void app_task_1(void *app_var)
{
    event_context_t *pObj = (event_context_t *)app_var;

    printf("app_task_1... \n");

    while(!pObj->task_1.stop)
    {
        if (0 == util_event_wait(pObj->event, 200)) {
            printf("app_task_1[%ld] \n", time(0));
        }
    }

    printf("app_task_1 leave... \n");
    pObj->task_1.stop_done = 1;
}

static void app_task_2(void *app_var)
{
    event_context_t *pObj = (event_context_t *)app_var;

    printf("app_task_2... \n");

    while(!pObj->task_2.stop)
    {
        printf("app_task_2[%ld] \n", time(0));
        util_event_post(pObj->event);
        util_task_wait_msecs(1000);
    }

    printf("app_task_2 leave... \n");
    pObj->task_2.stop_done = 1;
}

static void app_int_sig_handler(int sig)
{
    g_event_ctx.exit = 1;
    g_event_ctx.task_1.stop = 1;
    g_event_ctx.task_2.stop = 1;

    printf("app_int_sig_handler exit\n");
    // exit(0);
}

void test_event(void)
{
    int status = 0;

    printf("++++++++++++++++++++++++task++++++++++++++++++++++++\n");

    signal(SIGINT, app_int_sig_handler);

    g_event_ctx.exit = 0;

    /* create event */
    status = util_event_create(&g_event_ctx.event);
    if (0 != status) {
        return;
    }

    /* create task1 */
    util_task_create_params_t params;
    memset(&params, 0, sizeof(util_task_create_params_t));

    params.core_affinity = UTIL_TASK_AFFINITY_ANY;
    params.priority      = UTIL_TASK_PRI_LOWEST;
    params.task_main     = app_task_1;
    params.app_var       = &g_event_ctx;
    strncpy(params.task_name, "MutexTask1", UTIL_MAX_TASK_NAME);
    params.task_name[UTIL_MAX_TASK_NAME-1U] = (char)0;

    status = util_task_create(&g_event_ctx.task_1.task, &params);
    if (0 != status) {
        return;
    }

    g_event_ctx.task_1.stop_done = 0;
    g_event_ctx.task_1.stop      = 0;

    /* create task2 */
    params.core_affinity = UTIL_TASK_AFFINITY_ANY;
    params.priority      = UTIL_TASK_PRI_LOWEST;
    params.task_main     = app_task_2;
    params.app_var       = &g_event_ctx;
    strncpy(params.task_name, "MutexTask2", UTIL_MAX_TASK_NAME);
    params.task_name[UTIL_MAX_TASK_NAME-1U] = (char)0;

    status = util_task_create(&g_event_ctx.task_2.task, &params);
    if (0 != status) {
        return;
    }
    g_event_ctx.task_2.stop_done = 0;
    g_event_ctx.task_2.stop      = 0;

    while (!g_event_ctx.exit) {
        util_task_wait_msecs(100);
    }

    /* delete task */
    while(g_event_ctx.task_1.stop_done == 0) {
        util_task_wait_msecs(1000);
    }
    util_task_delete(&g_event_ctx.task_1.task);

    while(g_event_ctx.task_2.stop_done == 0) {
        util_task_wait_msecs(1000);
    }
    util_task_delete(&g_event_ctx.task_2.task);

    util_event_delete(&g_event_ctx.event);
}
