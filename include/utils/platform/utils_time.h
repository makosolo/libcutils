#ifndef H_UTILS_TIME_H
#define H_UTILS_TIME_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Wall clock time. Gets CLOCK_REALTIME on Posix.
 * @return current timestamp in milliseconds.
 */
uint64_t util_time_ms(void);

/**
 * Wall clock time. Gets CLOCK_REALTIME on Posix.
 * @return current timestamp in nanoseconds.
 */
uint64_t util_time_ns(void);

/**
 * Monotonic timer. Gets CLOCK_MONOTONIC on Posix
 * @return current timestamp in milliseconds.
 */
uint64_t util_time_mono_ms(void);

/**
 * Monotonic timer. Gets CLOCK_MONOTONIC on Posix
 * @return Current timestamp in nanoseconds.
 */
uint64_t util_time_mono_ns(void);

/**
 * @param millis milliseconds to sleep.
 * @return '0' on success, negative on failure.
 */
int util_time_sleep(uint64_t millis);

#ifdef __cplusplus
}
#endif

#endif // H_UTILS_TIME_H
