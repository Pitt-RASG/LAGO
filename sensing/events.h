#ifndef _EVENTS_H
#define _EVENTS_H

#include <stdint.h>

uint64_t event_type_code(const char *name);
const char *event_type_name(uint64_t code);

#endif // _EVENTS_H
