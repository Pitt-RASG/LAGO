/**
 * Energy reading for an ODROID with INA231 power sensors.
 *
 * Based on EnergyMon by Connor Imes
 * https://github.com/energymon/energymon/tree/master/odroid
 */

#include "power.h"

// Power sensor directories
static char *sensor_dirs[] = {A7_POWER, A15_POWER, MEM_POWER, GPU_POWER};

// Odroid power state
static odroid_state state;

/**
 * Poll all sensor files.
 */
void *poll_energy_sensors()
{
    char cdata[8];
    double sum_w = 0;
    uint64_t start_us;
    uint64_t elapsed_us;

    while (state.poll_sensors) {
        start_us = gettime_us();
        size_t i;
        for (i = 0; i < NUM_SENSORS; i++) {
            if (pread(state.sensor_fds[i], cdata, sizeof(cdata), 0) > 0) {
                sum_w += strtod(cdata, NULL);
            }
        }
        elapsed_us = gettime_us() - start_us;
        state.total_uj += sum_w * elapsed_us;

        usleep(UPDATE_INTERVAL);
    }
}

/**
 * Open all sensor files.
 */
void setup_energy_monitor()
{
    // initialize total energy estimate
    state.total_uj = 0; 
    state.poll_sensors = 1;
   
    // open individual sensor files
    size_t i;
    for (i = 0; i < NUM_SENSORS; i++) {
        state.sensor_fds[i] = open(sensor_dirs[i], O_RDONLY);
        if (state.sensor_fds[i] < 0) {
            perror("LAGO: Failed to open power sensors.");
        }
    }

    // poll sensors
    pthread_create(&state.thread, NULL, poll_energy_sensors, NULL);
}

/**
 * Close all sensor files.
 */
void close_energy_monitor()
{
    // stop polling sensors
    state.poll_sensors = 0;
    pthread_join(state.thread, NULL);

    // close individual sensor files
    size_t i;
    for (i = 0; i < NUM_SENSORS; i++) {
        close(state.sensor_fds[i]);
    }
}


/**
 * Read total energy.
 */
uint64_t read_total_energy()
{
    return state.total_uj;
}
