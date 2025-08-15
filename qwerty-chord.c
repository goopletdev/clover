#include "./clvrlib.h"
#include <fcntl.h>
#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

unsigned int keydown(int key_code, unsigned int chord, int* keys_down) {
    (*keys_down)++;

    if (chord & CANCEL_MASK) {
        return chord;
    }

    if (QWERTY_KEY_VALS[key_code]) {
        return chord | QWERTY_KEY_VALS[key_code];
    }

    return chord | CANCEL_MASK;
}

unsigned int keyup(unsigned int chord, int* keys_down) {
    if (--(*keys_down) == 0) {
        return chord & CANCEL_MASK ? 0U : chord | SEND_MASK;
    } else if (*keys_down < 0) {
        *keys_down = 0;
        return 0U;
    }

    return chord;
}

void send_chord(unsigned int chord, struct config cf) {
    printf("\r%s | ", paper_tape(chord));
    dict* entry = get(cf.dictionary->children, chord);
    char* trns = entry ? entry->translation : NULL;
    printf("%s\n", trns ? trns : pretty_chord(chord));
}

void event_loop(struct config cf) {
    int fd;
    struct input_event event;
    unsigned int chord = 0;
    int keys_down = 0;

    fd = open(cf.listeners[0], O_RDONLY);

    for (;;) {
        read(fd, &event, sizeof(event));
        if (event.type != 1) {
            continue;
        }
        putchar('\b');
        switch(event.value) {
            case 0:
                chord = keyup(chord, &keys_down);
                if (chord & SEND_MASK) {
                    send_chord(chord & (~SEND_MASK), cf);
                    chord = 0U;
                }
                break;
            case 1:
                chord = keydown(event.code, chord, &keys_down);
                break;
            default:
                continue;
        }
    }
}
/*
int main(int argc, char **argv) {
    int fd;
    struct input_event event;
    unsigned int chord = 0;
    int keys_down = 0;

    fd = open("/dev/input/event2", O_RDONLY);
    //fd = open(argv[1], O_RDONLY);
    //printf("\n");

    for (;;) {
        read(fd, &event, sizeof(event));
        if (event.type != 1) {
            continue;
        } 
        printf("\b");
        switch (event.value) {
            case 0:
                chord = keyup(chord, &keys_down);
                if (chord & SEND_MASK) {
                    send_chord(chord & (~SEND_MASK));
                    chord = 0U;
                }
                break;
            case 1:
                chord = keydown(event.code, chord, &keys_down);
                break;
            default:
                continue;
        }
    }

    return 0;
}

*/



