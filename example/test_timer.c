#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "platform/include/utils_task.h"
#include "platform/include/utils_timer.h"

typedef struct {
    util_task_t*    task;
    uint32_t		stop;
    uint32_t		stop_done;
} task_obj_t;

typedef struct
{
    task_obj_t timer_task;;
} timer_context_t;

timer_context_t g_timer_ctx;

static void app_timer_task(void *app_var)
{
    timer_context_t *pObj = (timer_context_t*)app_var;
    if (NULL == pObj) {
        return;
    }

    while(!pObj->timer_task.stop) {

    }

    pObj->timer_task.stop_done = 1;
}

void test_timer(void)
{
    printf("++++++++++++++++++++++++timer++++++++++++++++++++++++\n");

}
