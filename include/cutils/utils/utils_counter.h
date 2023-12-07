#ifndef H_UTILS_COUNTER_H
#define H_UTILS_COUNTER_H

#include <stdint.h>
#include <stdbool.h>

struct util_counter_s {
    uint64_t start;
    uint64_t period;
    uint64_t delay;
    bool     started;
};

typedef struct util_counter_s util_counter_t;

int             util_counter_create(util_counter_t *counter, uint64_t period);
int             util_counter_destroy(util_counter_t *counter);
void            util_counter_start(util_counter_t *counter, uint64_t count, uint64_t delay);
void            util_counter_restart(util_counter_t *counter, uint64_t count, uint64_t delay);
void            util_counter_stop(util_counter_t *counter);
bool            util_counter_started(util_counter_t *counter);
uint64_t        util_counter_remain(util_counter_t *counter, uint64_t count, bool reset);

#endif // H_UTILS_COUNTER_H
