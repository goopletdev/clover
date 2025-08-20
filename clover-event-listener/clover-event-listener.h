#ifndef CLOVER_EVENT_LISTENER_H
#define CLOVER_EVENT_LISTENER_H

#include "../clover-chord/clover-chord.h"
#include "../clover-hash/clover-hash.h"
#include <linux/input.h>
#include <stdlib.h>

/**
 * @brief
 * @param fd File descriptor of event location
 * @param chord Bitmask for steno chord
 * @param keys_down Pointer to int storing number of keys currently held
 */
unsigned int clover_handle_key_event(
    int fd, unsigned int chord, int* keys_down
);

#endif // CLOVER_EVENT_LISTENER_H