#ifndef CLOVER_EVENT_EMULATOR_H
#define CLOVER_EVENT_EMULATOR_H

#include <libevdev/libevdev-uinput.h> // libevdev_uinput_write_event
                                      // struct libevdev_uinput
#include <linux/input.h> // uses keyname macros e.g. KEY_0, KEY_SPACE
#include <stdio.h> // printf
#include <stdlib.h> // realloc, free

/**
 * Sends keyboard events to replicate a given string
 */
void send_string(struct libevdev_uinput* uinput_dev, const char* str);

#endif // CLOVER_EVENT_EMULATOR_H
