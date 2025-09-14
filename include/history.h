#ifndef CLOVER_HISTORY_H
#define CLOVER_HISTORY_H

#include <stdlib.h> // malloc, calloc, realloc

// #include "chord.h"
#include "dict.h"

typedef struct clover__history_elementT clover_history_element;
struct clover__history_elementT {
    // perhaps clover_histories should include chord strokes
    // clover_chord chord;
    struct clover__instructionT* instruction;
    struct clover__dictT* source_dict;
    struct clover__history_elementT* prev;
    struct clover__history_elementT* next;
};

typedef struct clover__historyT clover_history;
struct clover__historyT {
    int current_len;
    clover_history_element* head;
    clover_history_element* tail;
};

/**
 * Malloc a clover_history element w/ each field initialized to 0 or NULL
 */
clover_history* clover_history_init(void);

/**
 * Initialize a clover_history_element with each field set to NULL
 */
clover_history_element* clover_history_element_init(void);

/**
 * Add a clover_history_element 
 */
void clover_history_push(clover_history* history, clover_history_element* element);

/**
 * Remove and return the last element of a clover_history linked list
 */
clover_history_element* clover_history_pop(clover_history* history);

/**
 * Remove and return the first element of a clover_history linked list
 */
clover_history_element* clover_history_shift(clover_history* history);

#endif // CLOVER_HISTORY_H