#ifndef _DEF_H
#define _DEF_H

#include <inttypes.h>
#include <sys/types.h>

// Number of performance counters to track
#define NUM_COUNTERS 5

// Sensing sampling rate
#define SAMPLING_RATE 200000 // 200ms

// Predictor filename
#define PREDICTOR_FILE "predictor.py"

typedef struct lago_state {
    uint64_t counters[NUM_COUNTERS];
    int prediction;
    pid_t pid;
} lago_state;

#endif
