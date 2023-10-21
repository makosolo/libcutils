#ifndef H_UTILS_COUNTER_H
#define H_UTILS_COUNTER_H

#include <stdint.h>
#include <stdbool.h>

struct util_counter_s;
typedef struct util_counter_s util_counter_t;

util_counter_t* util_counter_create(uint64_t period);
void            util_counter_destroy(util_counter_t *counter);
void            util_counter_start(util_counter_t *counter, uint64_t count, uint64_t delay);
void            util_counter_restart(util_counter_t *counter, uint64_t count, uint64_t delay);
void            util_counter_stop(util_counter_t *counter);
bool            util_counter_started(util_counter_t *counter);
uint64_t        util_counter_remain(util_counter_t *counter, uint64_t count, bool reset);

#endif // H_UTILS_COUNTER_H
