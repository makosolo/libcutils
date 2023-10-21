#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <signal.h>

#include "platform/include/utils_task.h"

typedef struct
{
    util_task_t*    task;
    uint32_t		stop;
    uint32_t		stop_done;
} task_context_t;

task_context_t g_task_ctx;

static void app_task(void *app_var)
{
    task_context_t *pObj = (task_context_t *)app_var;

    printf("app_task... \n");

    while(!pObj->stop)
    {
        printf("app_task[%ld]++++++\n", time(0));
        util_task_wait_msecs(1000);
    }

    printf("app_task leave... \n");
    pObj->stop_done = 1;
}

static void app_int_sig_handler(int sig)
{
    g_task_ctx.stop = 1;

    printf("app_int_sig_handler exit\n");
    // exit(0);
}

void test_task(void)
{
    int status = 0;

    printf("++++++++++++++++++++++++task++++++++++++++++++++++++\n");

    signal(SIGINT, app_int_sig_handler);

    /* create task */
    util_task_create_params_t params;
    memset(&params, 0, sizeof(util_task_create_params_t));

    params.core_affinity = UTIL_TASK_AFFINITY_ANY;
    params.priority      = UTIL_TASK_PRI_LOWEST;
    params.task_main     = app_task;
    params.app_var       = &g_task_ctx;
    strncpy(params.task_name, "TaskTest", UTIL_MAX_TASK_NAME);
    params.task_name[UTIL_MAX_TASK_NAME-1U] = (char)0;

    status = util_task_create(&g_task_ctx.task, &params);
    if (0 != status) {
        return;
    }

    g_task_ctx.stop_done = 0;
    g_task_ctx.stop      = 0;

    while (!g_task_ctx.stop) {
        util_task_wait_msecs(100);
    }

    /* delete task */
    while(g_task_ctx.stop_done == 0) {
        util_task_wait_msecs(1000);
    }
    util_task_delete(&g_task_ctx.task);
}
