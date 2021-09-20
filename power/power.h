/**
 * Energy reading for an ODROID with INA231 power sensors.
 *
 * Based on EnergyMon by Connor Imes
 * https://github.com/energymon/energymon/tree/master/odroid
 */

#include <inttypes.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define NELEM(x)        (sizeof(x)/sizeof((x)[0]))

#define POWER_SENSOR(x) ("/sys/bus/i2c/drivers/INA231/0-" #x "/sensor_W")
#define A7_POWER        POWER_SENSOR(0045)
#define A15_POWER       POWER_SENSOR(0040)
#define MEM_POWER       POWER_SENSOR(0041)
#define GPU_POWER       POWER_SENSOR(0044)
#define UPDATE_INTERVAL 263808
#define NUM_SENSORS     4

typedef struct odroid_state {
    int sensor_fds[NUM_SENSORS]; // file descriptors for power sensors
    int poll_sensors;
    pthread_t thread;
    uint64_t total_uj; // total energy estimate
} odroid_state;

/**
 * Poll all sensor files.
 */
void *poll_energy_sensors();

/**
 * Open all sensor files.
 */
void setup_energy_monitor();

/**
 * Close all sensor files.
 */
void close_energy_monitor();

/**
 * Read total energy.
 */
uint64_t read_total_energy();
