/**
 * Utility functions.
 */

#include "utils.h"

static int little_cores[] = { 0, 1, 2, 3 };
static int big_cores[]    = { 4, 5, 6, 7 };

/**
 * Get clock time in microseconds.
 */
uint64_t gettime_us()
{
    struct timespec ts;
    uint64_t ns;

	clock_gettime(CLOCK_MONOTONIC, &ts);
    ns = ts.tv_sec * (uint64_t) ONE_BILLION + ts.tv_nsec;

	return ns / ONE_THOUSAND;
}

/**
 * Transfer to little core.
 */
void transfer_to_little(pid_t pid)
{
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);

    size_t i;
	for (i = 0; i < NELEM(little_cores); i++) {
		CPU_SET(little_cores[i], &cpuset);
	}

	if (sched_setaffinity(pid, sizeof(cpuset), &cpuset) < 0) {
		perror("LAGO: Failed to sched_setaffinity");
	}
}

/**
 * Transfer to big core.
 */
void transfer_to_big(pid_t pid)
{
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);

    size_t i;
	for (i = 0; i < NELEM(big_cores); i++) {
		CPU_SET(big_cores[i], &cpuset);
	}

	if (sched_setaffinity(pid, sizeof(cpuset), &cpuset) < 0) {
		perror("LAGO: Failed to sched_setaffinity");
	}
}
