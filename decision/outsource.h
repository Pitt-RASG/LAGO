#include <stdarg.h>
#include <stdio.h>

/**
 * Send arguments to the external decision module.
 */
void send_to_predictor(const char* fmt, ...);

/**
 * Receive output external decision module.
 */
void recv_from_predictor(const char* fmt, ...);

/**
 * Spawn external decision module.
 */
int spawn_predictor(const char* command);
