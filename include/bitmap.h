#ifndef CLOVER_BITMAP_H
#define CLOVER_BITMAP_H

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned long clover_hex_word;

struct clover__bitmapT {
    clover_hex_word* words;
    int len;
};

typedef struct clover__bitmapT clover_bitmap;

/**
 * Frees malloc'd .words
 */
int clover_free_bitmap(clover_bitmap m);

/**
 * Parses bitmap from space-deliniated hex string.
 * Expects exactly one non-hex value (e.g. newline) before null terminator
 */
clover_bitmap clover_parse_hexmap(const char* map);

/**
 * Initializes a clover_bitmap with len words
 */
clover_bitmap clover_init_empty_bitmap(int len);

/**
 * Sets bit to high in clover_bitmap at a given position.
 * Recursively adds clover_hex_word's as necessary.
 */
clover_bitmap clover_bitmap_add_value(clover_bitmap m, int val);

/**
 * Initializes a new clover_bitmap from array of bit positions
 */
clover_bitmap clover_bitmap_from_array(const int* vals, int len);

/**
 * Checks whether a given bit is high
 */
int clover_bitmap_bit_is_high(clover_bitmap m, int bit_position);

/**
 * Sets a bit to high, if bit is in range of bitmap
 */
clover_bitmap clover_bitmap_set_bit_high(clover_bitmap m, int bit_pos);

/**
 * Clears a bit, if in range of bitmap
 */
clover_bitmap clover_bitmap_clear_bit(clover_bitmap m, int bit_pos);

// NOT IMPLEMENTED:
int clover_bitmap_is_subset_of_bitmap(clover_bitmap subset, clover_bitmap set);
int clover_bitmaps_are_equivalent(clover_bitmap a, clover_bitmap b);
char* clover_hex_string_from_bitmap(clover_bitmap m);

#endif // CLOVER_BITMAP_H
