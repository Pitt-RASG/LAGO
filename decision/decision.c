#include "decision.h"

/**
 * Decision module.
 */
void decision(lago_state *state)
{
    if (state->prediction == 0) {
        transfer_to_little(state->pid);
    }
    else if (state->prediction == 1) {
        transfer_to_big(state->pid);
    }
}
