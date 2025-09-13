#ifndef CLOVER_ENGINE_H
#define CLOVER_ENGINE_H

#include <libevdev/libevdev-uinput.h>
#include <stdio.h>
#include "table.h"
#include "bitmap.h"
#include "silly-string.h"

typedef enum {
    UNKNOWN_MACRO,
    RETRO_INSERT_SPACE,
    RETRO_DELETE_SPACE,
    UNDO,
    REPEAT_LAST_STROKE,
    RETRO_TOGGLE_ASTERISK,
} clover_macro;

typedef enum {
    UNKNOWN_COMMAND,
    SUSPEND,
    RESUME,
    TOGGLE,
    ADD_TRANSLATION,
    LOOKUP,
    SUGGESTIONS,
    CONFIGURE,
    FOCUS,
    QUIT,
    SET_CONFIG, // takes args
} clover_command;

typedef struct clover__instructionT clover_instruction;
// i don't think deletedText will work correctly with unicode
// figure that out later ig
struct clover__instructionT {
    enum {
        NONE, ASCII, UNICODE, COMMAND, MACRO, META, DELETE, MOVEMENT,
    } type;
    union {
        char* inputText;
        char* deletedText;
        char* keyCombo;
        int* unicode;
        clover_command command;
        clover_macro macro;
        char* meta;
        int movement;
    } u;
    char* args;
    struct clover__instructionT* next;
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

void clover_engine_init();

void clover_free_history(clover_history* h);

void clover_instance_push_history(clover_instance* ci, clover_history* new);

void clover_engine_cleanup();

#endif // CLOVER_ENGINE_H
