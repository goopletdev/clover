#include <clvrlib.h>
#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int STENO_ORDER_SIZE = strlen(STENO_ORDER);
const int NUMBER_MIDDLE_KEYS = RIGHT_START - MIDDLE_START;
const int NUMBER_RIGHT_KEYS = STENO_ORDER_SIZE - RIGHT_START;
const unsigned int M_KEY_MASK = ((1U << NUMBER_MIDDLE_KEYS) - 1) << MIDDLE_START;
const unsigned int R_KEY_MASK = ((1U << NUMBER_RIGHT_KEYS) - 1) << RIGHT_START;
const unsigned int CANCEL_MASK = 1U << STENO_ORDER_SIZE;
const unsigned int SEND_MASK = CANCEL_MASK << 1;

const int QWERTY_KEY_VALUES[KEY_MAX + 1] = {
    [0 ... KEY_MAX] = -1,

    [KEY_Q] = 0, [KEY_A] = 1, [KEY_W] = 2, [KEY_S] = 3,
    [KEY_E] = 4, [KEY_D] = 5, [KEY_R] = 6, [KEY_F] = 7,

    [KEY_C] = 8, [KEY_V] = 9,
    [KEY_T] = 10, [KEY_G] = 10, [KEY_Y] = 10, [KEY_H] = 10,
    [KEY_N] = 11, [KEY_M] = 12,

    [KEY_U] = 13, [KEY_J] = 14, [KEY_I] = 15, [KEY_K] = 16,
    [KEY_O] = 17, [KEY_L] = 18, [KEY_P] = 19, [KEY_SEMICOLON] = 20,
    [KEY_LEFTBRACE] = 21, [KEY_APOSTROPHE] = 22
};

void check_memory_allocation(void* ptr, int exit_code) {
    if (ptr == NULL) {
        printf("Memory allocation failed");
        exit(exit_code);
    }
    return;
}

char* pretty_chord(unsigned int chord) {
    char* pretty = (char*)calloc(STENO_ORDER_SIZE, sizeof(char));
    check_memory_allocation(pretty, 1);

    int key_index = 0;
    for (int i = 0; i < STENO_ORDER_SIZE; i++) {
        if (chord & (1U << i)) {
            pretty[key_index++] = STENO_ORDER[i];
        } else if (
                i == RIGHT_START - 1
                && chord & R_KEY_MASK
                && chord & (~M_KEY_MASK)
        ) {
            pretty[key_index++] = '-';
        }
    }

    pretty = (char*)realloc(pretty, sizeof(char) * (key_index + 1));
    check_memory_allocation(pretty, 1);

    return pretty;
}

char* paper_tape(unsigned int chord) {
    char* tape = (char*)malloc(STENO_ORDER_SIZE * sizeof(char));
    for (int i = 0; i < STENO_ORDER_SIZE; i++) {
        if (chord & (1U << i)) {
            tape[i] = STENO_ORDER[i];
        } else {
            tape[i] = ' ';
        }
    }
    return tape;
}

int steno_index_of(char value, int start) {
    for (int i = start; i < STENO_ORDER_SIZE; i++) {
        if (STENO_ORDER[i] == value) {
            return i;
        }
    }
    return -1;
}

void print_bits(unsigned int arr) {
    for (int i = 0; i < 8 * sizeof(unsigned int); i++) {
        unsigned int mask = 1U << i;
        if (mask & arr) {
            printf("1");
        } else {
            printf("0");
        }
    }
    printf("\n");
}
    

int compare_chords(unsigned int chord1, unsigned int chord2) {
    unsigned int steno_mask = (1U << STENO_ORDER_SIZE) - 1;

    int comparison;
    for (unsigned int bitmask = 1; bitmask < steno_mask; bitmask <<= 1) {
        if (comparison = (bitmask & chord1) - (bitmask & chord2)) {
            bitmask = steno_mask & ~((bitmask << 1) - 1);
            if (comparison > 0) {
                if (bitmask & chord2) {
                    return -1;
                }
                return 1;
            }
            if (bitmask & chord1) {
                return 1;
            }
            return -1;
        }
    }
    return 0;
}

u_int_array parse_pretty_chord(char* chord, int* start, char terminator) {
    u_int_array id;
    id.size = 1;
    id.arr = (unsigned int*)calloc(id.size, sizeof(unsigned int));
    check_memory_allocation(id.arr, 1);

    int search_start = 0;
    for (char key; (key = chord[*start]) != terminator; (*start)++) {
        search_start = steno_index_of(key, search_start);
        if (search_start == -1) {
            switch (key) {
                case '-':
                    search_start = RIGHT_START;
                    break;
                case '/':
                    search_start = 0;
                    id.arr = (unsigned int*)realloc(id.arr, ++id.size * sizeof(unsigned int));
                    check_memory_allocation(id.arr, 1);
                    break;
                default:
                    printf("Error parsing chord \"%s\"\n", *chord);
                    exit(1);
            }
        } else {
            id.arr[id.size - 1] |= 1U << search_start++;
        }
    }

    return id;
}

int binary_dictionary_search(dictionary* dict, unsigned int chord, int start, int end) {
    if (start > end) {
        return -1;
    }
    int midpoint = start + ((end - start) / 2);
    int comparison = compare_chords(chord, dict[midpoint].id);
    if (comparison) {
        if (comparison > 0) {
            return binary_dictionary_search(dict, chord, midpoint + 1, end);
        } 
        return binary_dictionary_search(dict, chord, start, midpoint - 1);
    }
    return midpoint;
}

    
