#ifndef CLOVER_INSTRUCTION_H
#define CLOVER_INSTRUCTION_H

#include "chord.h"
#include "dict.h"
#include "gd-trie.h"
#include "gd-doubly-linked-list.h"

#include <stdint.h> // uintptr_t

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

typedef gd_dll clover_instruction;

typedef struct clover__instruction_nodeT clover_instruction_node;
struct clover__instruction_nodeT {
    clover_instruction_type type;
    union {
        char* string;
        clover_macro macro;
        clover_command command;
        char* meta;
    } u;
    char* args;
};

typedef struct clover__history_elementT clover_history_element;
struct clover__history_elementT {
    clover_instruction* instruction;
    clover_dict* source_dict;
};

typedef gd_dll clover_history;

typedef struct clover__settingsT clover_settings;
struct clover__settingsT {
    char* space_mode;
    int space_before;
};

typedef struct clover__instanceT clover_instance;
struct clover__instanceT {
    clover_history* history;
    clover_settings* settings;
    gd_trie* macros;
    gd_trie* commands;
};

clover_instance* clover_instance_init(void);
void clover_instance_free(clover_instance* i);

/**
 * initialize trie data structure for quick command and macro lookup
 */
gd_trie* clover_instruction_init_macros(void);
gd_trie* clover_instruction_init_commands(void);

void clover_instruction_free_node(void* instruction);
void clover_instruction_free(clover_instruction* inst);
void clover_history_free_element(void* el);
void clover_history_free(clover_history* hist);

clover_instruction_node* clover_instruction_from_brackets(
        const char* bracket_contents);
clover_instruction_node* clover_instruction_from_macro(
        const char* macro_contents, gd_trie* macro_trie);

clover_instruction* clover_instruction_from_dict(clover_dict* dict, clover_instance* instance);

#endif // CLOVER_INSTRUCTION_H
