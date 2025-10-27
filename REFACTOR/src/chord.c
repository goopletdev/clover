#include "chord.h"

#define L_KEY_MASK ((1U << MIDDLE_START) - 1)
#define M_KEY_MASK (((1U << NUMBER_MIDDLE_KEYS) - 1) << MIDDLE_START)
#define R_KEY_MASK (((1U << NUMBER_RIGHT_KEYS) - 1) << RIGHT_START)

int clover_pretty_chord(char* buffer, size_t buffer_len, clover_chord chord) {
    chord &= STENO_MASK;
    int separator = (chord & R_KEY_MASK) && !(chord & M_KEY_MASK) ? 1 : 0;
    size_t max = buffer_len - 1;

    size_t buffer_pos = 0;
    int bit_pos;
    while ((chord & L_KEY_MASK) && (buffer_pos < max)) {
        bit_pos = __builtin_ctz(chord);
        chord &= ~(1U << bit_pos);
        buffer[buffer_pos++] = STENO_ORDER[bit_pos];
    }
    if (separator && buffer_pos < max) {
        buffer[buffer_pos++] = '-';
    }
    while (chord && (buffer_pos < max)) {
        bit_pos = __builtin_ctz(chord);
        chord &= ~(1U << bit_pos);
        buffer[buffer_pos++] = STENO_ORDER[bit_pos];
    }
    buffer[buffer_pos] = '\0';
    return buffer_pos + 1;
}

int clover_paper_tape(char* buffer, size_t buffer_len, clover_chord chord) {
    size_t max = (buffer_len - 1) < STENO_ORDER_LEN ? buffer_len - 1 : STENO_ORDER_LEN;
    for (size_t i = 0; i < max; i++) {
        buffer[i] = chord & (1U << i) ? STENO_ORDER[i] : ' ';
    }
    buffer[max] = '\0';
    return STENO_ORDER_LEN + 1;
}

int clover_chord_compare(clover_chord chord1, clover_chord chord2) {
    unsigned int bitmask = (chord1 & STENO_MASK) ^ (chord2 & STENO_MASK);
    if (!bitmask) {
        return 0;
    }
    bitmask &= ~(bitmask - 1);

    if ((int)(bitmask & chord1)) { 
        // chord1 has the earlier different value
        if ((bitmask - 1) & chord2) {
            // chord2 doesn't have subsequent keys. 
            // chord2 precedes chord1 in steno order.
            return 1;
        }
        // chord2 has subsequent keys.
        // chord2 follows chord1 in steno order.
        return -1;
    } 
    // chord2 has the earlier different value
    if ((bitmask - 1) & chord1) {
        // chord1 doesn't have subsequent keys.
        // chord1 precedes chord2 in steno order.
        return -1;
    }
    // chord1 has subsequent keys.
    // chord1 follows chord2 in steno order.
    return 1;
}

int clover__steno_index_of(char value, int start) {
    for (size_t i = start; i < STENO_ORDER_LEN; i++) {
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
                printf("Unrecognized char [%c] in chord [%s]\n", key, chord);
                return 0;
            }
            parsed_chord |= 1U << search_start++;
        }
    }
    return parsed_chord;
}

clover_chord* clover_parse_compound_chord(clover_chord* buffer, size_t buffer_len, char* chord) {
    memset(buffer, 0, sizeof(clover_chord) * buffer_len);
    size_t buffer_position = 0;
    int search_start = 0;
    char key;

    for (size_t i = 0; (key = chord[i]); i++) {
        if (key == '/') {
            if (++buffer_position >= (buffer_len - 1)) {
                break;
            }
            search_start = 0;
        } else if (key == '-') {
            search_start = RIGHT_START;
        } else {
            search_start = clover__steno_index_of(key, search_start);
            if (search_start == -1) {
                // unrecognized character
                printf("Unrecognized char [%c] in compound chord [%s]\n", key, chord);
                buffer[buffer_position] = 0;
                return buffer;
            }
            buffer[buffer_position] |= 1U << search_start++;
        }
    }
    buffer[buffer_position + 1] = 0;

    return buffer;
}

size_t clover_chord_len(clover_chord* chord) {
    size_t i = 0;
    while (chord[i]) i++;
    return i;
}
