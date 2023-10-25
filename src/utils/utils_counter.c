#include <stdlib.h>
#include <stdint.h>

#include "utils_counter.h"

int util_counter_create(util_counter_t *counter, uint64_t period)
{
    if (NULL == counter) {
        return -1;
    }
    
    counter->start    = 0;
    counter->delay    = 0;
    counter->period   = period;
    counter->started  = false;

    return 0;
}

int util_counter_destroy(util_counter_t *counter)
{
    if (NULL == counter) {
        return -1;
    }

    util_counter_t *temp = counter;

    counter = NULL;
    free(temp);

    return 0;
}

void util_counter_start(util_counter_t *counter, uint64_t count, uint64_t delay)
{
    if (NULL != counter && !counter->started) {
        counter->started = true;
        counter->start   = count;
        counter->delay   = delay;
    }
}

void util_counter_restart(util_counter_t *counter, uint64_t count, uint64_t delay)
{
    if (NULL != counter) {
        counter->started = true;
        counter->start   = count;
        counter->delay   = delay;
    }
}

void util_counter_stop(util_counter_t *counter)
{
    if (NULL != counter) {
        counter->started = false;
    }
}

bool util_counter_started(util_counter_t *counter)
{
    if (NULL != counter) {
        return counter->started;
    }

    return false;
}

uint64_t util_counter_remain(util_counter_t *counter, uint64_t count, bool reset)
{
    uint64_t result = 0;

    if (NULL == counter || counter->period == 0 || !counter->started) {
        return 0;
    }

    uint64_t value = count >= counter->start? counter->start: UINT64_MAX - counter->start;

    result = (count >= value + counter->period + counter->delay? 0 :
                (count - value - counter->period - counter->delay));

    if (result == 0) {
        if (!reset) {
            counter->started = false;
        }
        else {
            counter->start = count;
        }
        counter->delay = 0;
    }

    return result;
}
