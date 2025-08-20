#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./clover-chord.h"

const int STENO_ORDER_SIZE = strlen(STENO_ORDER);
const int NUMBER_MIDDLE_KEYS = RIGHT_START - MIDDLE_START;
const int NUMBER_RIGHT_KEYS = STENO_ORDER_SIZE - RIGHT_START;
const unsigned int L_KEY_MASK = (1U << MIDDLE_START) - 1;
const unsigned int M_KEY_MASK = ((1U << NUMBER_MIDDLE_KEYS) - 1) << MIDDLE_START;
const unsigned int R_KEY_MASK = ((1U << NUMBER_RIGHT_KEYS) - 1) << RIGHT_START;
const unsigned int CANCEL_MASK = 1U << STENO_ORDER_SIZE;
const unsigned int SEND_MASK = CANCEL_MASK << 1;
const unsigned int STENO_MASK = (1U << STENO_ORDER_SIZE) - 1;

int clover_chord_is_ready(unsigned int chord) {
    return chord & SEND_MASK;
}

unsigned int clover_chord_set_ready(unsigned int chord) {
    return chord | SEND_MASK;
}

int clover_chord_is_canceled(unsigned int chord) {
    return chord & CANCEL_MASK;
}

unsigned int clover_chord_set_canceled(unsigned int chord) {
    return chord | CANCEL_MASK;
}

unsigned int clover_chord_value(unsigned int chord) {
    return chord & STENO_MASK;
}

int clover__steno_index_of(char value, int start) {
    for (int i = start; i < STENO_ORDER_SIZE; i++) {
        if (STENO_ORDER[i] == value) {
            return i;
        }
    }
    return -1;
}

int clover_chord_size(unsigned int chord) {
    return __builtin_popcount(chord & STENO_MASK);
}

char* clover_pretty_chord(unsigned int chord) {
    chord &= STENO_MASK;
    int separator = (chord & R_KEY_MASK) && !(chord & M_KEY_MASK) ? 1 : 0;
    char* pretty = (char*)malloc
        ((__builtin_popcount(chord) + separator + 1) * sizeof(char));

    if (pretty == NULL) {
        printf("Memory allocation failed");
        exit(1);
    }

    int pretty_pos = 0;
    int bit_pos;
    while (chord & L_KEY_MASK) {
        bit_pos = __builtin_ctz(chord);
        chord &= ~(1U << bit_pos);
        pretty[pretty_pos++] = STENO_ORDER[bit_pos];
    }
    if (separator) {
        pretty[pretty_pos++] = '-';
    }
    while (chord) {
        bit_pos = __builtin_ctz(chord);
        chord &= ~(1U << bit_pos);
        pretty[pretty_pos++] = STENO_ORDER[bit_pos];
    }
    pretty[pretty_pos] = '\0';
    return pretty;
}

char* clover_paper_tape(unsigned int chord) {
    char* tape = (char*)malloc((STENO_ORDER_SIZE + 1) * sizeof(char));
    for (int i = 0; i < STENO_ORDER_SIZE; i++) {
        tape[i] = chord & (1U << i) ? STENO_ORDER[i] : ' ';
    }
    tape[STENO_ORDER_SIZE] = '\0';
    return tape;
}

int clover_chord_compare(unsigned int chord1, unsigned int chord2) {
    int comparison;
    for (unsigned int bitmask = 1; bitmask < STENO_MASK; bitmask <<= 1) {
        if ((comparison = ((bitmask & chord1) - (bitmask & chord2)))) {
            bitmask = STENO_MASK & ~((bitmask << 1) - 1);
            if (comparison > 0) {
                return bitmask & chord2 ? -1 : 1;
            }
            return bitmask & chord1 ? 1 : -1;
        }
    }
    return 0;
}

/**
 * TODO: fix
 * i swear there's a way to make this work without a loop
 */
int clover__chord_compare(unsigned int chord1, unsigned int chord2) {
    unsigned int bitmask = (chord1 &= STENO_MASK) ^ (chord2 &= STENO_MASK);
    if (!bitmask) {
        return 0;
    }
    bitmask &= -bitmask;

    if ((int)(bitmask & chord1)) { // chord1 has the earlier different value
        if (~((bitmask << 1) - 1) & chord2) {
            return -1;
        }
        return 1;
    } 
    if (~((bitmask << 1) - 1) & chord1) {
        return 1;
    }
    return -1;
}

unsigned int clover_parse_chord(char* chord) {
    unsigned int parsed_chord = 0;
    int search_start = 0;
    char key;
    for (int i=0; (key = chord[i]); i++) {
        if (key == '-') {
            search_start = RIGHT_START;
        } else {
            search_start = clover__steno_index_of(key, search_start);
            if (search_start == -1) {
                search_start = 0;
                continue;
            }
            parsed_chord |= 1U << search_start++;
        }
    }
    return parsed_chord;
}

unsigned int* clover_parse_compound_chord(char* chord, int* size) {
    *size = 0;
    unsigned int* parsed_chord = (unsigned int*)calloc(1, sizeof(int));
    int start = 0;
    char* buffer = (char*)calloc(strlen(chord) + 1, sizeof(char));
    if (buffer == NULL) {
        printf("Buffer allocation failed\n");
        exit(1);
    }
    for (int i = 0; chord[i]; i++) {
        if (chord[i] == '/') {
            parsed_chord = (unsigned int*)realloc(
                parsed_chord, (*size + 1) * sizeof(int)
            );
            if (parsed_chord == NULL) {
                printf("Memory allocation failed");
                exit(1);
            }
            parsed_chord[(*size)++] = clover_parse_chord(&buffer[start]);
            start = i + 1;
        } else {
            buffer[i] = chord[i];
        }
    }
    parsed_chord = (unsigned int*)realloc(parsed_chord, (*size + 1) * sizeof(int));
    if (parsed_chord == NULL) {
        printf("Memory allocation failed");
        exit(1);
    }
    parsed_chord[(*size)++] = clover_parse_chord(&buffer[start]);
    free(buffer);
    return parsed_chord;
}