#ifndef CLOVER_EVENT_EMULATOR_H
#define CLOVER_EVENT_EMULATOR_H

#include <libevdev/libevdev-uinput.h>

/**
 * Sends keyboard events to replicate a given string
 */
void send_string(struct libevdev_uinput* uinput_dev, const char* str);

#endif // CLOVER_EVENT_EMULATOR_H
