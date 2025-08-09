#include <clvrlib.h>
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

    int stenindex = QWERTY_KEY_VALUES[key_code];

    if (stenindex == -1) {
        return chord | CANCEL_MASK;
    } 

    return chord | (1U << stenindex);
}

unsigned int keyup(unsigned int chord, int* keys_down) {
    if (--(*keys_down) == 0) {
        if (!(chord & CANCEL_MASK)) {
            return chord | SEND_MASK;
        }
        return 0U;
    } else if (*keys_down < 0) {
        *keys_down = 0;
        return 0U;
    }

    return chord;
}

int main(int arc, char **argv) {
    int fd;
    struct input_event event;
    int return_code;
    unsigned int chord;
    int keys_down = 0;

    fd = open(argv[1], O_RDONLY);
    for (;;) {
        read(fd, &event, sizeof(event));
        if (event.type != 1) {
            continue;
        }
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
}





