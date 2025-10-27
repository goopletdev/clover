#ifndef CLOVER_JSON_H
#define CLOVER_JSON_H

#include <stdio.h> // printf, FILE type, fopen, fclose
#include <string.h> //memset

#include "chord.h"
#include "dict.h"

#define CHAR_BUFFER_SIZE 512

/**
 * @brief Parses Plover-style JSON steno dictionary into a clover_dict object
 */
clover_dict* clover_json_parse(const char* file_path, clover_dict* d);

#endif // CLOVER_JSON_H
