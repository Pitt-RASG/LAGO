#ifndef _PERF_H
#define _PERF_H

#include <linux/perf_event.h>
#include <linux/hw_breakpoint.h>

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>

#include "../utils/def.h"
#include "events.h"

/**
 * Struct definition for unimplemented library function perf_event_open.
 * See manual for perf_event_open(2).
 */
struct read_format {
	uint64_t value;
};

/**
 * Set up a raw event counter.
 */
void configure_raw_counter(size_t num, uint64_t hardware_id, pid_t pid);

/**
 * Parse event list (e.g., "l1d_cache_refill,mem_access,br_pred")
 */
void parse_event_list(char *event_list, size_t num);

/**
 * Configure sensing module.
 */
void configure_sensing(char *tracked_events, pid_t pid);

/**
 * Read performance counter data
 */
void poll_sensing_counters(lago_state *state);

/**
 * Struct definition for perf event.
 */
struct perf_info {
	int fd;
	uint64_t id;
	uint64_t val;
	uint64_t code;
	const char *name;
};

// Explicit counters
struct perf_info *counters;
size_t num_counters;

// Data for counters to be written into
struct read_format *events;
size_t events_size;

#endif // _PERF_H
