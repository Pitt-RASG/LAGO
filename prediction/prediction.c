#include "prediction.h"

/**
 * Configure sensing module.
 */
void configure_prediction()
{
    char predictor[512];
    snprintf(predictor, sizeof(predictor), "python3 ../prediction/%s", PREDICTOR_FILE);
    if (!spawn_predictor(predictor)) {
        perror("LAGO: Failed to spawn predictor.");
    }
}

/**
 * Prediction module.
 */
void predict(lago_state *state)
{
    char *fmt = "%lld,%llf,%lld,%llf,%llf";
    int prediction;
    
    // send performance counters to external predictor
    send_to_predictor(fmt, 
                    state->counters[0],
                    state->counters[1], 
                    state->counters[2], 
                    state->counters[3], 
                    state->counters[4]
                    );

    // receive output from external predictor
    recv_from_predictor("%d", &prediction);

    state->prediction = prediction;
}
