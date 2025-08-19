#ifndef CLOVER_CHORD_H
#define CLOVER_CHORD_H

#define STENO_ORDER "#STKPWHRAO*EUFRPBLGTSDZ"
#define MIDDLE_START 8
#define RIGHT_START 13

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