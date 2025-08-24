#ifndef CLOVER_JSON_H
#define CLOVER_JSON_H

#include <stdio.h> // printf, FILE type,
                   // fopen, fclose
#include <string.h> // memset

#include "chord.h" // clover_chord type
#include "hash.h" // clover_parse_compound_chord, 
                  // clover_push_entry, 
                  // clover_dict type

#define CHAR_BUFFER_SIZE 256


/**
 * Parses Plover-style JSON steno dictionary into a clover_dict object
 * @warning Have not tested on erroneous json files,
 *      but it will definitely only work currently with
 *      key-value pairs where both the key and the value 
 *      are simple string literals
 */
clover_dict* clover_parse_dictionary(const char* file_path);

#endif
