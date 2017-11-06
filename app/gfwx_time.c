
#include "gfwx_time.h"

#ifndef _WIN32

#include <time.h>
#include <sys/time.h>

uint64_t gfwx_GetTime(void)
{
	uint64_t ticks = 0;
#if defined(__linux__) || defined(__APPLE__)
	struct timespec ts;

	if (!clock_gettime(CLOCK_MONOTONIC_RAW, &ts))
		ticks = (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000);

#elif defined(_WIN32)
	ticks = (uint64_t) GetTickCount();
#else
	/**
	* FIXME: this is relative to the Epoch time, and we
	* need to return a value relative to the system uptime.
	*/
	struct timeval tv;

	if (!gettimeofday(&tv, NULL))
		ticks = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);

#endif
	return ticks;
}

#else

uint64_t gfwx_GetTime(void)
{
	return (uint64_t) GetTickCount64();
}

#endif
