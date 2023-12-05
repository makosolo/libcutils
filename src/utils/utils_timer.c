#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "utils_timer.h"

static int util_timer_insert(util_timer_list_t **head, util_timer_data_t *data)
{
    util_timer_data_t **next_data = head;

    if (NULL == head) {
        *head = data;
        return 0;
    }

    for (; ; next_data = &(*next_data)->next) {
        if (NULL == *next_data) {
            data->next = NULL;
            *next_data = data;
            break;
        }
        
        if (data->expire + data->dealy + data->birthtime < (*next_data)->expire + (*next_data)->dealy + (*next_data)->birthtime) {
            data->next = *next_data;
            *next_data = data;
            break;
        }
    }

    // for (util_timer_data_t **i = head; ; i = &(*i)->next) {
    //     if (NULL == *i) {
    //         break;
    //     }

    //     printf("util_timer_insert: expire=%d \n", (*i)->expire);
    // }

    return 0;
}

static int util_timer_clear(util_timer_t *timer)
{
    util_timer_data_t *data = timer->list;

    for (; ; data = data->next) {
        if (NULL == data) {
            break;
        }

        /* remove expired timer from list */
        timer->list = data->next;

        free(data);
    }

    return 0;
}

int util_timer_create(util_timer_t *timer, OnUtilGetTimestampCallback callback)
{
    if (NULL == timer || NULL == callback) {
        return -1;
    }

    memset(timer, 0, sizeof(util_timer_t));
    timer->onGetTimestamp = callback;

    return 0;
}

int util_timer_destroy(util_timer_t *timer)
{
    if (NULL == timer) {
        return -1;
    }

	util_timer_clear(timer);

    memset(timer, 0, sizeof(util_timer_t));

    return 0;
}

int util_timer_start(util_timer_t *timer, OnUtilTimerCallback callback, void *arg, uint32_t dealy, uint32_t expire, bool repeat)
{
    util_timer_data_t *data = NULL;
    
    if (NULL == timer || NULL == callback) {
        return -1;
    }

    data = (util_timer_data_t*)malloc(sizeof(util_timer_data_t));
    if (NULL != data) {
        data->next      = NULL;
		data->callback  = callback;
		data->arg       = arg;
        data->dealy     = dealy;
		data->expire    = expire;
		data->repeat    = repeat;
		data->birthtime = timer->onGetTimestamp();

        /* insert timer. */
        util_timer_insert(&timer->list, data);

        return 0;
    }

    return -2;
}

uint64_t util_timer_timeout(util_timer_t *timer)
{
    util_timer_data_t *data = NULL;

    if (NULL == timer) {
        return 0;
    }

    for (data = timer->list; ; data = data->next) {
        if (NULL == data) {
            return 0;
        }

        uint32_t deadline = data->expire + data->dealy + data->birthtime;

        if (timer->onGetTimestamp() < deadline) {
            return (uint64_t)(deadline - timer->onGetTimestamp());
        }

        /* remove expired timer from list */
        timer->list = data->next;

        if (data->repeat) {
            data->dealy     = 0;
            data->birthtime = timer->onGetTimestamp();
            util_timer_insert(&timer->list, data);
        }

        if (NULL != data->callback) {
            data->callback(data->arg);
        }

        /* delay free */
        if (!data->repeat) {
            free(data);
        }
    }

    return 0;
}
