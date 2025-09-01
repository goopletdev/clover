#ifndef CLOVER_ENGINE_H
#define CLOVER_ENGINE_H

#include <libevdev/libevdev-uinput.h>
#include "table.h"
#include "bitmap.h"

typedef struct clover__historyT clover_history;
struct clover__historyT {
    enum {
        INPUT, COMMAND,
    } chordType;
    struct {
        char* inputText;
        char* deletedText;
        clover_dict* source_dict;
    } translation;
    struct clover__historyT* previous;
    struct clover__historyT* next;
};

typedef struct clover__instanceT clover_instance;
struct clover__instanceT {
    struct libevdev_uinput* virtkb_dev;
    struct {
        int kbd_fd;
        clover_bitmap keys;
        clover_bitmap state;
    } keyboard;
    struct {
        int max_size;
        int current_size;
        clover_history* first;
        clover_history* current;
    } history;
    clover_dict* root;
};

void clover_free_history(clover_history* h);

void clover_instance_push_history(clover_instance* ci, clover_history new);

#endif // CLOVER_ENGINE_H