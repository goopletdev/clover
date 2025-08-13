#include <clvrlib.h>
#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int STENO_ORDER_SIZE = strlen(STENO_ORDER);
const int NUMBER_MIDDLE_KEYS = RIGHT_START - MIDDLE_START;
const int NUMBER_RIGHT_KEYS = STENO_ORDER_SIZE - RIGHT_START;
const unsigned int L_KEY_MASK = (1U << MIDDLE_START) - 1;
const unsigned int M_KEY_MASK = ((1U << NUMBER_MIDDLE_KEYS) - 1) << MIDDLE_START;
const unsigned int R_KEY_MASK = ((1U << NUMBER_RIGHT_KEYS) - 1) << RIGHT_START;
const unsigned int CANCEL_MASK = 1U << STENO_ORDER_SIZE;
const unsigned int SEND_MASK = CANCEL_MASK << 1;
const unsigned int STENO_MASK = (1U << STENO_ORDER_SIZE) - 1;

const int QWERTY_KEY_VALS[KEY_MAX + 1] = {
    [0 ... KEY_MAX] = 0,

    [KEY_Q] = 1 << 0, [KEY_A] = 1 << 1, 
    [KEY_W] = 1 << 2, [KEY_S] = 1 << 3,
    [KEY_E] = 1 << 4, [KEY_D] = 1 << 5, 
    [KEY_R] = 1 << 6, [KEY_F] = 1 << 7,

    [KEY_C] = 1 << 8, [KEY_V] = 1 << 9,
    [KEY_T] = 1 << 10, [KEY_G] = 1 << 10, 
    [KEY_Y] = 1 << 10, [KEY_H] = 1 << 10,
    [KEY_N] = 1 << 11, [KEY_M] = 1 << 12,

    [KEY_U] = 1 << 13, [KEY_J] = 1 << 14, 
    [KEY_I] = 1 << 15, [KEY_K] = 1 << 16,
    [KEY_O] = 1 << 17, [KEY_L] = 1 << 18, 
    [KEY_P] = 1 << 19, [KEY_SEMICOLON] = 1 << 20,
    [KEY_LEFTBRACE] = 1 << 21, [KEY_APOSTROPHE] = 1 << 22
};

/*
    // for use with parse_json_key()
unsigned int** init_steno_map(void) {
    unsigned int* left = (unsigned int*)calloc(128, sizeof(unsigned int));
    unsigned int* middle = (unsigned int*)calloc(128, sizeof(unsigned int));
    unsigned int* right = (unsigned int*)calloc(128, sizeof(unsigned int));
    
    for (int i = 0, int key; key = (int)STENO_ORDER[i]; i++) {
        if (i < MIDDLE_START) {
            left[key] = 1U << i;
        } else if (i < RIGHT_START) {
            middle[key] = 1U << i;
        } else {
            right[key] = 1U << i;
        }
    }

    unsigned int** steno_map = (unsigned int**)calloc(3, sizeof(unsigned int*));
    steno_map[0] = left;
    steno_map[1] = middle;
    steno_map[2] = right;
    return steno_map;
}

const unsigned int** STENO_MAP = init_steno_map();
*/

void check_memory_allocation(void* ptr, int exit_code) {
    if (ptr == NULL) {
        printf("Memory allocation failed");
        exit(exit_code);
    }
    return;
}

int get_chord_size(unsigned int n) {
    // this is Brian Kernighan's algorithm. should use __builtin_popcount() 
    // from GCC extension instead
    int count = 0;
    while (n) {
        n &= n - 1;
        count++;
    }
    return count;
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

char* pretty_chord_fast(unsigned int chord) {
    unsigned int right_keys = chord & R_KEY_MASK;
    unsigned int middle_keys = chord & M_KEY_MASK;
    unsigned int left_keys = chord & L_KEY_MASK;
    int separator = 0;
    if (right_keys && !middle_keys) {
        separator++;
    }

    char* pretty = (char*)malloc(
        (__builtin_popcount(chord) + separator) * sizeof(char)
    );

    check_memory_allocation(pretty, 1);

    int position = 0;
    unsigned int bitmask = 0;
    while (bitmask ^= left_keys & -left_keys) { 
        left_keys ^= bitmask;
        pretty[position++] = STENO_ORDER[__builtin_ctz(bitmask)];
    }
    if (separator) {
        pretty[position++] = '-';
    } else {
        while (bitmask ^= middle_keys & -middle_keys) {
            middle_keys ^= bitmask;
            pretty[position++] = STENO_ORDER[__builtin_ctz(bitmask)];
        }
    }
    while (bitmask ^= right_keys & -right_keys) {
        right_keys ^= bitmask;
        pretty[position++] = STENO_ORDER[__builtin_ctz(bitmask)];
    }

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
    int comparison;
    for (unsigned int bitmask = 1; bitmask < STENO_MASK; bitmask <<= 1) {
        if (comparison = (bitmask & chord1) - (bitmask & chord2)) {
            bitmask = STENO_MASK & ~((bitmask << 1) - 1);
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

int compare_chords_fast(unsigned int chord1, unsigned int chord2) {
    unsigned int bitmask = chord1 ^ chord2;
    if (!bitmask) {
        return 0;
    }
    bitmask &= -bitmask;

    if (((int)(bitmask & chord1) - (bitmask & chord2)) > 0) {
        if (STENO_MASK & ~((bitmask << 1) - 1) & chord2) {
            return -1;
        }
        return 1;
    } 
    if (STENO_MASK & ~((bitmask << 1) - 1) & chord1) {
        return 1;
    }
    return -1;
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

/*
    // this is janky, compare speed to parse_pretty_chord later
u_int_array parse_json_key(char* chord, int* start, char terminator) {
    u_int_array id;
    id.size = 1;
    id.arr = (unsigned int*)calloc(id.size, sizeof(unsigned int));
    check_memory_allocation(id.arr, 1);

    int steno_section = 0;
    char key;
    unsigned int bit;

    for (; key = chord[*start];) {
        if (bit = STENO_MAP[steno_section][(int)key]) {
            id.arr[id.size - 1] |= bit;
            (*start)++;
            continue;
        } 
        switch (key) {
            case '"':
                (*start)++;
                return id;
            case '-':
                steno_section = 2;
                (*start)++;
                continue;
            case '/':
                steno_section = 0;
                (*start)++;
                id.arr = (unsigned int*)realloc(
                    id.arr, ++id.size*sizeof(unsigned int)
                );
                continue;
            default:
                steno_section++;
                continue;
        }
    }
}
*/

   
