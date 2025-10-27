#ifndef CLOVER_CHORD_H
#define CLOVER_CHORD_H

#include <stdio.h> // printf
#include <stdlib.h> // type size_t
#include <string.h> // memset, for parsing compound chord

#define STENO_ORDER "#STKPWHRAO*EUFRPBLGTSDZ"
#define STENO_ORDER_LEN (sizeof(STENO_ORDER) - 1)
#define MIDDLE_START 8
#define RIGHT_START 13

#define NUMBER_MIDDLE_KEYS (RIGHT_START - MIDDLE_START)
#define NUMBER_RIGHT_KEYS (STENO_ORDER_LEN - RIGHT_START)

// chord value mask:
#define STENO_MASK ((1U << STENO_ORDER_LEN) - 1)
// flags mask:
#define FLAGS_MASK (~STENO_MASK)

typedef unsigned int clover_chord;

/**
 * returns chord bitmask without flags
 */
static inline clover_chord clover_chord_value(clover_chord chord) {
    return chord & STENO_MASK;
}

/**
 * Returns the number of steno keys in a chord.
 * Ignores flag bits.
 */
static inline int clover_chord_size(clover_chord chord) {
    return __builtin_popcount(chord & STENO_MASK);
}

/**
 * Returns:
 * 0: chord1 and chord2 are identical
 * positive value: chord1 comes after chord2 in steno order.
 * negative value: chord1 precedes chord2 in steno order.
 */
int clover_chord_compare(clover_chord chord1, clover_chord chord2);

/**
 * @brief Stringifies a chord into a steno stroke
 * @return The number of chars that would have been written,
 * had buffer_len been sufficiently large
 */
int clover_pretty_chord(char* buffer, size_t buffer_len, clover_chord chord);

/**
 * @brief Converts a chord into classic whitespace-padded steno machine output
 * @return The number of chars that would have been written, 
 * had buffer_len been sufficiently large
 * @see clover_pretty_chord()
 */
int clover_paper_tape(char* buffer, size_t buffer_len, clover_chord chord);

/**
 * @brief Parses the key of a dictionary entry into an clover_chord.
 */
clover_chord clover_parse_chord(char* chord);

/**
 * @brief Parses a compound chord (i.e. one w/ one or more strokes) 
 * into a buffer. 
 * @return reference to buffer
 * @see clover_parse_chord()
 */
clover_chord* clover_parse_compound_chord(clover_chord* buffer, size_t buffer_len, char* chord);

/**
 *
 */
size_t clover_chord_len(clover_chord* compound_chord);

#endif // CLOVER_CHORD_H
