/**
 * Utility functions.
 */

#ifndef _UTILS_H
#define _UTILS_H

#define _GNU_SOURCE
#include <inttypes.h>
#include <sched.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <time.h>

#define ONE_THOUSAND 1000
#define ONE_MILLION  1000000
#define ONE_BILLION  1000000000

#define NELEM(x) (sizeof(x)/sizeof((x)[0]))

/**
 * Get clock time in microseconds.
 */
uint64_t gettime_us();

/**
 * Transfer to little core.
 */
void transfer_to_little(pid_t pid);

/**
 * Transfer to big core.
 */
void transfer_to_big(pid_t pid);

#endif
