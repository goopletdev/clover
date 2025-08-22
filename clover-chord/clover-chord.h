#ifndef CLOVER_CHORD_H
#define CLOVER_CHORD_H

#define STENO_ORDER "#STKPWHRAO*EUFRPBLGTSDZ"
#define STENO_ORDER_LEN (sizeof(STENO_ORDER) - 1)
#define MIDDLE_START 8
#define RIGHT_START 13

#define NUMBER_MIDDLE_KEYS (RIGHT_START - MIDDLE_START)
#define NUMBER_RIGHT_KEYS (STENO_ORDER_LEN - RIGHT_START)

// flag masks:
#define CANCEL_MASK (1U << STENO_ORDER_LEN)
#define SEND_MASK (CANCEL_MASK << 1)
// chord value mask:
#define STENO_MASK ((1U << STENO_ORDER_LEN) - 1)

typedef unsigned int clover_chord;

/**
 * Checks "send" flag to see whether chord is ready to be sent
 */
static inline int clover_chord_is_ready(clover_chord chord) {
    return chord & SEND_MASK;
}

/**
 * Returns new clover_chord with "send" flag set to true
 */
static inline clover_chord clover_chord_set_ready(clover_chord chord) {
    return chord | SEND_MASK;
}

/**
 * Checks whether the "cancel" flag is set
 */
static inline int clover_chord_is_canceled(clover_chord chord) {
    return chord & CANCEL_MASK;
}

/**
 * Returns a new clover_chord chord with the "cancel" flag set
 */
static inline clover_chord clover_chord_set_canceled(clover_chord chord) {
    return chord | CANCEL_MASK;
}

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
 * @return Dynamically allocated string
 * @note uses malloc; caller must free() return value
 */
char* clover_pretty_chord(clover_chord chord);

/**
 * @brief Converts a chord into classic whitespace-padded steno machine output
 * @return Dynamically allocated string
 * @note uses malloc; caller must free() return value
 * @see clover_pretty_chord()
 */
char* clover_paper_tape(clover_chord chord);

/**
 * Parses the key of a dictionary entry into an clover_chord.
 */
clover_chord clover_parse_chord(char* chord);

/**
 * Parses a compound chord (i.e. one with multiple strokes)
 * int* size MUST be a pointer to an int set to 0
 * Uses malloc; be sure to free return value
 */
clover_chord* clover_parse_compound_chord(char* chord, int* size);

#endif // CLOVER_CHORD_H
