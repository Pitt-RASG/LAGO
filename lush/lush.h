#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <time.h>

#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include "../utils/def.h"
#include "../decision/decision.h"
#include "../prediction/prediction.h"
#include "../power/power.h"
#include "../utils/utils.h"
#include "../sensing/sensing.h"
#include "../sensing/events.h"

// Barrier
static pthread_barrierattr_t attr;
static pthread_barrier_t *barrier;

// Traced process
static pid_t child;

// List of events to track
//static char tracked_events[] = "cpu_cycles,inst_retired,l2d_cache,l2d_cache_refill,br_mis_pred";
