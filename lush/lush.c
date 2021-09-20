#include "lago.h"

static uint64_t start_uj, end_uj, start_usec, end_usec;
static char *tracked_events = NULL;

/**
 * Configure the barrier used for event reporting, so that when performace
 * counting is enabled, event counting can enabled before the traced process
 * starts.
 */
static void setup_barrier()
{
	barrier = mmap(NULL, sizeof(pthread_barrier_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if (barrier == MAP_FAILED) {
		perror("LAGO: Failed mmap.");
	}

	pthread_barrierattr_init(&attr);
	pthread_barrierattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
	pthread_barrier_init(barrier, &attr, 2);
}

/**
 * Spawn the child process which will be run, wait on the barrier for events
 * to become enabled, and then execve() the child.
 */
static void spawn_child(char *command)
{
    char *args[] = {command, NULL};

	child = fork();

	if (child < 0) {
		perror("LAGO: Failed to fork.");
	}
    
	if (child == 0) {
		prctl(PR_SET_PDEATHSIG, SIGKILL);
		pthread_barrier_wait(barrier);
		if (execv(args[0], args) != 0) {
			perror("LAGO: Failed to execute command.");
            exit(0);
		}
	}
    
    // start at LITTLE core
	transfer_to_little(child);
}

/**
 * Signal handler for exiting process.
 */
static void sigchld_handler(int signal)
{
	// get end time/energy
    end_usec = gettime_us();
    end_uj = read_total_energy();

    // close energy sensors
    close_energy_monitor();

    // print statistics 
    printf("Total runtime: %f secs\n", (double) (end_usec-start_usec) / ONE_MILLION);
    printf("Average power in Watts: %f\n", 
        (end_uj-start_uj) / (double) (end_usec-start_usec));

	exit(0);
}

/**
 * Parse command-line arguments.
 */
static void parse_cmd_arguments(int argc, char *argv[])
{
    int opt;
    while ((opt = getopt(argc, argv, ":p:e:")) != -1) {
    switch (opt) {
        case 'e':
            // select events to monitor
            tracked_events = optarg;
            break;
        case 'p':
            // execute prediction module
            break;
        case '?':
            // unknown option
            printf("Unknown option: %c\n", optopt);
            break;
        }
    }
}

int main(int argc, char *argv[])
{
    lago_state state;

    // parse command-line arguments
    parse_cmd_arguments(argc, argv);

	// set up the execution barrier and get the child ready
	setup_barrier();
	spawn_child(argv[argc-1]);
	signal(SIGCHLD, sigchld_handler);
    state.pid = child;
    
    // execute framework modules (if events are provided)
    if (tracked_events) {
        // configure sensing module
        configure_sensing(tracked_events, child);
        
        // configure prediction module
        configure_prediction();
    }

	// kick off execution
	pthread_barrier_wait(barrier);

	// setup energy monitor
	setup_energy_monitor();

    // get start time/energy
    start_usec = gettime_us();
    start_uj = read_total_energy();

    if (tracked_events) {
        // execute framework modules
        while (1) {
            // poll sensing counters
            poll_sensing_counters(&state);

            // prediction module
            predict(&state);

            // decision module
            decision(&state);

            // sleep
            usleep(SAMPLING_RATE);
        }
    }
    else {
        // execute framework without modules
        while (1) {
            // sleep
            usleep(SAMPLING_RATE);
        }
    }

	return 0;
}
