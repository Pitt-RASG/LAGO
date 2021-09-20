#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "events.h"

#define NELEM(x) (sizeof(x)/sizeof((x)[0]))

struct event_type {
	const char *	event_name;
	uint64_t	    event_code;
};

/**
 * Abridged collection of useful PMU events. See Armv8.1-M
 * Performance Monitoring User Guide, pp. 24 for more details.
 */
const struct event_type armv8pmu_events[] = {
	{"l1i_cache_refill",        0x0001},
	{"l1d_cache_refill",		0x0003},
	{"l1d_cache",			    0x0004},
	{"ld_retired",			    0x0006},
	{"st_retired",			    0x0007},
	{"inst_retired",		    0x0008},
	{"exc_taken",			    0x0009},
	{"exc_return",			    0x000a},
	{"pc_write_retired",		0x000c},
	{"br_immed_retired",		0x000d},
	{"br_return_retired",		0x000e},
	{"unaligned_ldst_retired",	0x000f},
	{"br_mis_pred",			    0x0010},
	{"cpu_cycles",			    0x0011},
	{"br_pred",			        0x0012},
	{"mem_access",			    0x0013},
	{"l1i_cache",			    0x0014},
	{"l1d_cache_wb",		    0x0015},
	{"l2d_cache",			    0x0016},
	{"l2d_cache_refill",		0x0017},
	{"l2d_cache_wb",		    0x0018},
	{"bus_access",			    0x0019},
	{"memory_error",		    0x001a},
	{"inst_spec",			    0x001b},
	{"bus_cycles",			    0x001d},
};

/**
 * See Cortex-A7 MPCore r0p3 Technical Reference Manual,
 * ss. 11.5 "Events" for more details.
 */
const struct event_type armv7pmu_events[] = {
	{"l1i_cache_refill",		0x0001},
	{"l1i_tlb_refill",		    0x0002},
	{"l1d_cache_refill",		0x0003},
	{"l1d_cache",			    0x0004},
	{"ld_retired",			    0x0006},
	{"st_retired",			    0x0007},
	{"inst_retired",		    0x0008},
	{"br_immed_retired",		0x000d},
	{"br_mis_pred",			    0x0010},
	{"cpu_cycles",			    0x0011},
	{"br_pred",			        0x0012},
	{"mem_access",			    0x0013},
	{"l1i_cache",			    0x0014},
	{"l1d_cache_wb",		    0x0015},
	{"l2d_cache",			    0x0016},
	{"l2d_cache_refill",		0x0017},
	{"l2d_cache_wb",		    0x0018},
	{"bus_access",			    0x0019},
	{"bus_cycles",			    0x001d},
};

#define events armv7pmu_events

uint64_t event_type_code(const char *name)
{
    size_t i;
	for (i = 0; i < NELEM(events); ++i) {
		if (strcmp(name, events[i].event_name) == 0) {
			return events[i].event_code;
		}
	}

	return -1;
}

const char *event_type_name(uint64_t code)
{
    size_t i;
	for (i = 0; i < NELEM(events); ++i) {
		if (code == events[i].event_code) {
			return events[i].event_name;
		}
	}

	return NULL;
}
