#ifndef CLOVER_TABLE_H
#define CLOVER_TABLE_H

#include <stdlib.h> // malloc, calloc, realloc
#include <string.h> // uses strlen, strcpy
#include "chord.h"

typedef struct clover__dictT clover_dict;
struct clover__dictT {
    clover_chord id;
    struct {
        int len;
        char** entries;
    } translations;
    clover_dict* parent;
    struct {
        int size;
        int capacity;
        clover_dict** dicts;
    } children;
};

/**
 * @brief Makes new dict with child capacity of 8.
 * @return malloc'd clover_dict with calloc'd children.dicts
 * @note uses malloc and calloc; caller must free()
 */
clover_dict* clover_table_init_dict(clover_chord id, clover_dict* parent);

/**
 * @brief Adds a new translation entry to given dict. 
 * @param d Target dictionary for adding translation
 * @param trns Translation to copy into dictionary. Caller should free() trns
 * @return Returns the quantity of entries in dict.
 * @note Copies given string to new entry; user should free trns
 */
int clover_table_dict_add_translation(clover_dict* d, char* trns);

/**
 * @brief Probes from target index until finding an entry with target id
 * or finding an empty slot. 
 * @param parent clover_dict to probe for child
 * @param target_id id to search for
 * @return Child element with target_id or NULL
 */
clover_dict* clover_dict_get(clover_dict* parent, clover_chord target_id);

/**
 * @brief Recursively finds/adds new entries until adding translation 
 * @return Returns pointer to entry with added value
 */
clover_dict* clover_dict_push_entry(
    clover_dict* d, clover_chord* id, int id_size, char* translation
);

/**
 * @brief recursively finds dictionary with target id, or returns null
 */
clover_dict* clover_seek(clover_dict* d, clover_chord* id, int id_len);

#endif // CLOVER_TABLE_H