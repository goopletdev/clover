#ifndef CLOVER_JSON_H
#define CLOVER_JSON_H

#include "../clover-hash/clover-hash.h"
#include "../clover-chord/clover-chord.h"

#define CHAR_BUFFER_SIZE 256


/**
 * Parses Plover-style JSON steno dictionary into a clover_dict object
 */
clover_dict* clover_parse_dictionary(const char* file_path);

#endif