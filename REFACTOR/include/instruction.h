#ifndef CLOVER_INSTRUCTION_H
#define CLOVER_INSTRUCTION_H

#include "chord.h"
#include "dict.h"
#include "gd-trie.h"
#include "gd-doubly-linked-list.h"
#include "history.h"

typedef enum {
    UNKNOWN_MACRO,
    RETRO_INSERT_SPACE,
    RETRO_DELETE_SPACE,
    UNDO,
    REPEAT_LAST_STROKE,
    RETRO_TOGGLE_ASTERISK,
} clover_macro;

typedef enum {
    UNKNOWN_COMMAND,
    SUSPEND,
    RESUME,
    TOGGLE,
    ADD_TRANSLATION,
    LOOKUP,
    SUGGESTIONS,
    CONFIGURE,
    FOCUS,
    QUIT,
    SET_CONFIG,
} clover_command;

typedef enum {
    NONE, STRING, COMMAND, MACRO, META, DELETE, SPACE,
} clover_instruction_type;

typedef struct gd_dll clover_instruction;

struct clover_instruction_node {
    clover_instruction_type type;
    union {
        char* string;
        clover_macro macro;
        clover_command command;
        char* meta;
        char* delete;
    } u;
    char* args;
};

/**
 * initialize trie data structure for quick command and macro lookup
 */
gd_trie* clover_instruction_init_macros(void);
gd_trie* clover_instruction_init_commands(void);

void clover_instruction_free(clover_instruction* inst);

clover_macro clover_instruction_lookup_macro(char* translation);
clover_command clover_instruction_lookup_command(char* translation);

clover_instruction_node* clover_instruction_from_brackets(
        const char* bracket_contents);
clover_instruction_node* clover_instruction_from_macro(
        const char* macro_contents);

clover_instruction* clover_instruction_from_dict(clover_dict* dict, clover_chord chord);

#endif // CLOVER_INSTRUCTION_H
