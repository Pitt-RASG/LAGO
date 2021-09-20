#include "sensing.h"

/**
 * Syscall wrapper for unimplemented library function perf_event_open.
 * See manual for perf_event_open(2).
 */
inline int perf_event_open(struct perf_event_attr *attr, pid_t pid, int cpu, int group_fd, unsigned long flags)
{
	int ret = syscall(__NR_perf_event_open, attr, pid, cpu, group_fd, flags);
	if (ret < 0) perror("LAGO: Failed to call perf_event_open.");
	return ret;
}

/**
 * Set up a raw event counter.
 */
void configure_raw_counter(size_t num, uint64_t hardware_id, pid_t pid)
{
	struct perf_event_attr pe;
    memset(&pe, 0, sizeof(pe));
	pe.type = PERF_TYPE_RAW;
	pe.size = sizeof(struct perf_event_attr);
	pe.config = hardware_id;
	pe.exclude_kernel = 1;
	pe.exclude_hv = 1;

	// install event counter
	counters[num].fd = perf_event_open(&pe, pid, -1, -1, PERF_FLAG_FD_CLOEXEC);
}

/**
 * Parse event list (e.g., "l1d_cache_refill,mem_access,br_pred").
 */
void parse_event_list(char *event_list, size_t num)
{
	uint64_t code = -1;
	char *token = strtok(event_list, ",");

	if (token == NULL) {
		num_counters = num;
		counters = calloc(num_counters, sizeof(struct perf_info));
		events_size = sizeof(struct read_format)*num_counters;
		events = (struct read_format *) calloc(1, events_size);
		return;
	}

	if ((code = event_type_code(token)) == -1) {
		fprintf(stderr, "Unknown event type %s\n", token);
		exit(-1);
	}

	// store event code on stack and recurse to parse next value
	parse_event_list(NULL, num + 1);

	// write back code and name to counters
	counters[num].code = code;
	counters[num].name = event_type_name(code);
}

/**
 * Configure sensing module.
 */
void configure_sensing(char *tracked_events, pid_t pid)
{
	// set up the event list
	parse_event_list(tracked_events, 0);

	// configure the perf file descriptors
    size_t i;
	for (i = 0; i < num_counters; i++) {
		configure_raw_counter(i, counters[i].code, pid);
	}
}

/**
 * Read performance counter data
 */
void poll_sensing_counters(lago_state *state)
{
    size_t i;
	for (i = 0; i < num_counters; i++) {
		if (read(counters[i].fd, &events[i], sizeof(struct read_format)) < 0) {
			perror("LAGO: Failed to read sensors");
		}
        state->counters[i] = events[i].value;
	}
}
