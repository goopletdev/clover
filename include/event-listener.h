#ifndef CLOVER_EVENT_LISTENER_H
#define CLOVER_EVENT_LISTENER_H

#include "../clover-hash/clover-hash.h"
#include <linux/input.h> // key name macros, eg KEY_Q, KEY_A
                         // struct input_event
#include <stdio.h> // putchar
#include <unistd.h> // read

#include "chord.h" // clover_chord type
                   // clover_chord_is_canceled
                   // clover_chord_set_ready
                   // clover_chord_set_canceled

/**
 * @brief
 * @param fd File descriptor of event location
 * @param chord Bitmask for steno chord
 * @param keys_down Pointer to int storing number of keys currently held
 */
clover_chord clover_handle_key_event(
    int fd, clover_chord chord, int* keys_down
);

#endif // CLOVER_EVENT_LISTENER_H
