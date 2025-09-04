#ifndef CLOVER_ENGINE_H
#define CLOVER_ENGINE_H

#include <libevdev/libevdev-uinput.h>
#include "table.h"
#include "bitmap.h"

typedef struct clover__instructionT clover_instruction;
// i don't think deletedText will work correctly with unicode
// figure that out later ig
struct clover__instructionT {
    enum {
        NONE, ASCII, UNICODE, COMMAND, DELETE, 
    } type;
    union {
        char* inputText;
        char* deletedText;
        char* keyCombo;
        int* unicode;
    } u;
};

typedef struct clover__historyT clover_history;
struct clover__historyT {
    struct {
        clover_instruction* instructions;
        int instructions_len;
        clover_dict* source_dict;
    } translation;
    struct clover__historyT* prev;
    struct clover__historyT* next;
};

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

void clover_free_history(clover_history* h);

void clover_instance_push_history(clover_instance* ci, clover_history* new);

#endif // CLOVER_ENGINE_H
