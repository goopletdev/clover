#include "./clover-event-listener.h"

#include <stdio.h>
#include <unistd.h>

const unsigned int clover__QWERTY_KEY_VALS[KEY_MAX + 1] = {
    [0 ... KEY_MAX] = 0,

    // left hand keys:
    [KEY_Q] = 1 << 0, [KEY_A] = 1 << 1, 
    [KEY_W] = 1 << 2, [KEY_S] = 1 << 3,
    [KEY_E] = 1 << 4, [KEY_D] = 1 << 5, 
    [KEY_R] = 1 << 6, [KEY_F] = 1 << 7,

    // middle keys (vowels, asterisk):
    [KEY_C] = 1 << 8, [KEY_V] = 1 << 9,
    [KEY_T] = 1 << 10, [KEY_G] = 1 << 10, // These four keys all map to the
    [KEY_Y] = 1 << 10, [KEY_H] = 1 << 10, // middle key of a steno machine
    [KEY_N] = 1 << 11, [KEY_M] = 1 << 12,

    // right hand keys: 
    [KEY_U] = 1 << 13, [KEY_J] = 1 << 14, 
    [KEY_I] = 1 << 15, [KEY_K] = 1 << 16,
    [KEY_O] = 1 << 17, [KEY_L] = 1 << 18, 
    [KEY_P] = 1 << 19, [KEY_SEMICOLON] = 1 << 20,
    [KEY_LEFTBRACE] = 1 << 21, [KEY_APOSTROPHE] = 1 << 22
};

unsigned int clover__keydown(
    int key_code, unsigned int chord, int* keys_down
) {
    (*keys_down)++;

    if (clover_chord_is_canceled(chord)) {
        return chord;
    }

    if (clover__QWERTY_KEY_VALS[key_code]) {
        return chord | clover__QWERTY_KEY_VALS[key_code];
    }

    return clover_chord_set_canceled(chord);
}

unsigned int clover__keyup(unsigned int chord, int* keys_down) {
    if (--(*keys_down) == 0) {
        return clover_chord_is_canceled(chord) ? 0U : clover_chord_set_ready(chord);
    } else if (*keys_down < 0) {
        *keys_down = 0;
        return 0U;
    }

    return chord;
}

unsigned int clover_handle_key_event(int fd, unsigned int chord, int* keys_down) {
    struct input_event event;
    read(fd, &event, sizeof(event));
    if (event.type != 1) {
        return chord;
    }
    putchar('\b');
    switch (event.value) {
        case 0:
            chord = clover__keyup(chord, keys_down);
            break;
        case 1:
            chord = clover__keydown(event.code, chord, keys_down);
    }
    return chord;
}