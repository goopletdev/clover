#ifndef CLOVER_CHORD_H
#define CLOVER_CHORD_H

#define STENO_ORDER "#STKPWHRAO*EUFRPBLGTSDZ"
#define MIDDLE_START 8
#define RIGHT_START 13

/**
 * Checks "send" flag to see whether chord is ready to be sent
 */
int clover_chord_is_ready(unsigned int chord);

/**
 * Returns new unsigned int with "send" flag set to true
 */
unsigned int clover_chord_set_ready(unsigned int chord);

/**
 * Checks whether the "cancel" flag is set
 */
int clover_chord_is_canceled(unsigned int chord);

/**
 * Returns a new unsigned int chord with the "cancel" flag set
 */
unsigned int clover_chord_set_canceled(unsigned int chord);

/**
 * returns chord bitmask without flags
 */
unsigned int clover_chord_value(unsigned int chord);

/**
 * Returns the number of steno keys in a chord.
 * Ignores flag bits.
 */
int clover_chord_size(unsigned int chord);

/**
 * Returns:
 * 0: chord1 and chord2 are identical
 * positive value: chord1 comes after chord2 in steno order.
 * negative value: chord1 precedes chord2 in steno order.
 */
int clover_chord_compare(unsigned int chord1, unsigned int chord2);

/**
 * @brief Stringifies a chord into a steno stroke
 * @return Dynamically allocated string
 * @note uses malloc; caller must free() return value
 */
char* clover_pretty_chord(unsigned int chord);

/**
 * @brief Converts a chord into classic whitespace-padded steno machine output
 * @return Dynamically allocated string
 * @note uses malloc; caller must free() return value
 * @see clover_pretty_chord()
 */
char* clover_paper_tape(unsigned int chord);

/**
 * Parses the key of a dictionary entry into an unsigned int.
 */
unsigned int clover_parse_chord(char* chord);

/**
 * Parses a compound chord (i.e. one with multiple strokes)
 * int* size MUST be a pointer to an int set to 0
 * Uses malloc; be sure to free return value
 */
unsigned int* clover_parse_compound_chord(char* chord, int* size);

#endif // CLOVER_CHORD_H