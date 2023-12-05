// https://github.com/tezc/sc/tree/master
#include <assert.h>
#include <errno.h>
#include <sys/time.h>
#include <time.h>

#include "utils_time.h"

uint64_t util_time_ms(void)
{
	int rc;
	struct timespec ts;

	rc = clock_gettime(CLOCK_REALTIME, &ts);
	assert(rc == 0);
	(void) rc;

	return ts.tv_sec * 1000 + (uint64_t) (ts.tv_nsec / 1000000);
}

uint64_t util_time_ns(void)
{
	int rc;
	struct timespec ts;

	rc = clock_gettime(CLOCK_REALTIME, &ts);
	assert(rc == 0);
	(void) rc;

	return ts.tv_sec * (uint64_t) 1000000000 + ts.tv_nsec;
}

uint64_t util_time_mono_ms(void)
{
	int rc;
	struct timespec ts;

	rc = clock_gettime(CLOCK_MONOTONIC, &ts);
	assert(rc == 0);
	(void) rc;

	return (uint64_t) ((uint64_t) ts.tv_sec * 1000 +
			   (uint64_t) ts.tv_nsec / 1000000);
}

uint64_t util_time_mono_ns(void)
{
	int rc;
	struct timespec ts;

	rc = clock_gettime(CLOCK_MONOTONIC, &ts);
	assert(rc == 0);
	(void) rc;

	return ((uint64_t) ts.tv_sec * 1000000000 + (uint64_t) ts.tv_nsec);
}

int util_time_sleep(uint64_t millis)
{
	int rc;
	struct timespec t, rem;

	rem.tv_sec = (time_t) millis / 1000;
	rem.tv_nsec = (long) (millis % 1000) * 1000000;

	do {
		t = rem;
		rc = nanosleep(&t, &rem);
	} while (rc != 0 && errno == EINTR);

	return rc;
}
