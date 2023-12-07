#ifndef H_UTILS_TIMER_H
#define H_UTILS_TIMER_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*OnUtilTimerCallback)(void *arg);
typedef uint64_t (*OnUtilGetTimestampCallback)(void);

struct util_timer_data_s {
	struct util_timer_data_s* next;
	uint64_t				  birthtime;
	uint32_t				  dealy;
	uint32_t				  expire;
	bool					  repeat;
	OnUtilTimerCallback		  callback;
	void*					  arg;
};
typedef struct util_timer_data_s util_timer_data_t;
typedef struct util_timer_data_s util_timer_list_t;

struct util_timer_s {
	util_timer_list_t* list;
	OnUtilGetTimestampCallback onGetTimestamp;
};
typedef struct util_timer_s util_timer_t;

/**
 * Create timer
 *
 * @param timer     timer
 * @param callback  get timestamp callback function
 *
 * @return 0 succeed, other fail
 */
int util_timer_create(util_timer_t *timer, OnUtilGetTimestampCallback callback);

/**
 * Destroy timer.
 * @param t timer
 *
 * @return 0 succeed, other fail
 */
int util_timer_destroy(util_timer_t *timer);

/**
 * Start timer
 * @param timer     timer
 * @param callback  callback when time runs out
 * @param arg       user arguments
 * @param dealy     delayed trigger time, millisecond
 * @param expire    expiration time, millisecond
 * @param repeat    repeat
 * 
 * @return 0 succeed, other fail
 */
int util_timer_start(util_timer_t *timer, OnUtilTimerCallback callback, void* arg, uint32_t dealy, uint32_t expire, bool repeat);

/**
 * Checks timeouts for each timeout.
 *
 * @param timer     timer
 *
 * @return remaining time, millisecond
 */
uint64_t util_timer_timeout(util_timer_t *timer);

#ifdef __cplusplus
}
#endif

#endif // H_UTILS_TIMER_H
