#ifndef CLOVER_HASH_H
#define CLOVER_HASH_H

#include "../clover-chord/clover-chord.h"
// the above include statement is currently there for
// convenient access to clover_parse_chord in the testing suite,
// as well as for convenient error messages

/**
 * Dictionary object that holds a translation of a steno stroke,
 * with a reference to its parent and child entries
 */
typedef struct clover__dictT clover_dict;


// ##################
// #    getters:    #
// ##################

/**
 * Getter;
 * Returns the id of a given entry
 */
const unsigned int clover_dict_id(clover_dict* d);

/**
 * Getter;
 * Returns the translation of a given entry
 */
const char* clover_dict_translation(clover_dict* d);

/**
 * Getter;
 * returns number of child entries
 */
const int clover_dict_size(clover_dict* d);

/**
 * Getter;
 * Returns pointer to given entry's parent
 */
const clover_dict* clover_dict_parent(clover_dict* d);

// ##################
// #  dict methods  #
// ##################

/**
 * Makes a new clover_dict from a given steno key and translation,
 * and returns a pointer to it. 
 * Root dictionary should have NULL parent
 */
clover_dict* clover_init_dict(
    unsigned int id, 
    char* translation, 
    clover_dict* parent // set to null for root dict
);

/**
 * Returns 1 if given clover_dict has an element with the given id,
 * returns 0 if it does not
 */
int clover_has(clover_dict* d, unsigned int id);

/**
 * Gets an entry by its steno chord id in a given clover_dict,
 * or returns NULL
 */
clover_dict* clover_get(clover_dict* d, unsigned int id);

/**
 * Recursively adds a multi-stroke steno translation
 */
clover_dict* clover_push_entry(
    clover_dict* d,
    unsigned int* id,
    int id_size,
    const char* translation
);

#endif // CLOVER_HASH_H