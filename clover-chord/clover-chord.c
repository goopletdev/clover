#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./clover-chord.h"

#define L_KEY_MASK ((1U << MIDDLE_START) - 1)
#define M_KEY_MASK (((1U << NUMBER_MIDDLE_KEYS) - 1) << MIDDLE_START)
#define R_KEY_MASK (((1U << NUMBER_RIGHT_KEYS) - 1) << RIGHT_START)

char* clover_pretty_chord(clover_chord chord) {
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

char* clover_paper_tape(clover_chord chord) {
    char* tape = (char*)malloc((STENO_ORDER_LEN + 1) * sizeof(char));
    for (int i = 0; i < STENO_ORDER_LEN; i++) {
        tape[i] = chord & (1U << i) ? STENO_ORDER[i] : ' ';
    }
    tape[STENO_ORDER_LEN] = '\0';
    return tape;
}

int clover_chord_compare(clover_chord chord1, clover_chord chord2) {
    int comparison;
    for (clover_chord bitmask = 1; bitmask < STENO_MASK; bitmask <<= 1) {
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
int clover__chord_compare(clover_chord chord1, clover_chord chord2) {
    unsigned int bitmask = (chord1 &= STENO_MASK) ^ (chord2 &= STENO_MASK);
    if (!bitmask) {
        return 0;
    }
    bitmask &= ~(bitmask - 1);

    if ((int)(bitmask & chord1)) { 
        // chord1 has the earlier different value
        if (~((bitmask << 1) - 1) & chord2) {
            // chord2 doesn't have subsequent keys. 
            // chord2 precedes chord1 in steno order.
            return 1;
        }
        // chord2 has subsequent keys.
        // chord 2 follows chord1 in steno order.
        return -1;
    } 
    // chord2 has the earlier different value
    if (~((bitmask << 1) - 1) & chord1) {
        // chord1 doesn't have subsequent keys.
        // chord1 precedes chord2 in steno order.
        return -1;
    }
    // chord1 has subsequent keyskeys.
    // chord1 precedes 
    return 1;
}

int clover___chord_compare(clover_chord chord1, clover_chord chord2) {
    unsigned int bitmask = (chord &= STENO_MASK) ^ (chord2 &= STENO_MASK);
    if (!(bitmask &= ~(bitmask - 1))) {
        return 0;
    }
    if (bitmask & chord1) {
        return ~(bitmask - 1) & chord2 ? 1 : -1;
    }
    return ~(bitmask - 1) & chord1 ? -1 : 1;
}

int clover__steno_index_of(char value, int start) {
    for (int i = start; i < STENO_ORDER_LEN; i++) {
        if (STENO_ORDER[i] == value) {
            return i;
        }
    }
    return -1;
}

clover_chord clover_parse_chord(char* chord) {
    clover_chord parsed_chord = 0;
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

clover_chord* clover_parse_compound_chord(char* chord, int* size) {
    *size = 0;
    clover_chord* parsed_chord = (clover_chord*)calloc(1, sizeof(int));
    int start = 0;
    char* buffer = (char*)calloc(strlen(chord) + 1, sizeof(char));
    if (buffer == NULL) {
        printf("Buffer allocation failed\n");
        exit(1);
    }
    for (int i = 0; chord[i]; i++) {
        if (chord[i] == '/') {
            parsed_chord = (clover_chord*)realloc(
                parsed_chord, (*size + 1) * sizeof(int)
            );
            if (parsed_chord == NULL) {
                printf("Memory allocation failed\n");
                exit(1);
            }
            parsed_chord[(*size)++] = clover_parse_chord(&buffer[start]);
            start = i + 1;
        } else {
            buffer[i] = chord[i];
        }
    }
    parsed_chord = (clover_chord*)realloc(parsed_chord, (*size + 1) * sizeof(int));
    if (parsed_chord == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    parsed_chord[(*size)++] = clover_parse_chord(&buffer[start]);
    free(buffer);
    return parsed_chord;
}
