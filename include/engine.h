#ifndef CLOVER_ENGINE_H
#define CLOVER_ENGINE_H

#include <libevdev/libevdev-uinput.h>
#include <stdio.h>
#include "dict.h"
#include "bitmap.h"
typedef struct clover__instanceT clover_instance;
struct clover__instanceT {
    enum {
        STENO, PASSTHROUGH,
    } mode;
    struct {
        enum {
            BEFORE, AFTER,
        } space_position;
        int space_adjustment;
    } input_settings;


    struct {
        struct libevdev_uinput* virtkb_dev;
        clover_bitmap state;
    } virtual_kbd;
    struct {
        int kbd_fd;
        clover_bitmap keys;
        clover_bitmap state;
    } keyboard;
    struct {
        int max_len;
        int current_len;
        clover_history* head;
        clover_history* tail;
    } history;
    clover_dict* root;
};

void clover_engine_init();

void clover_free_history(clover_history* h);

void clover_instance_push_history(clover_instance* ci, clover_history* new);

void clover_engine_cleanup();

#endif // CLOVER_ENGINE_H
