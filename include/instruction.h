#ifndef CLOVER_INSTRUCTION_H
#define CLOVER_INSTRUCTION_H

#include "chord.h"
#include "dict.h"
#include "silly-string.h"

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

typedef struct clover__instructionT clover_instruction;
struct clover__instructionT {
    enum {
        NONE, ASCII, UNICODE, COMMAND, MACRO, META, DELETE, MOVEMENT,
    } type;
    union {
        char* inputText;
        char* deletedText;
        char* keyCombo;
        int* unicode;
        clover_command command;
        clover_macro macro;
        char* meta;
        int movement;
    } u;
    char* args;
    struct clover__instructionT* prev;
    struct clover__instructionT* next;
};

/**
 * initialize module trie data structures for command and macro lookup
 */
void clover_instruction_start(void);

void clover_instruction_free(clover_instruction* inst);

void clover_instruction_cleanup(void);

clover_macro clover_instruction_lookup_macro(char* translation);

clover_command clover_instruction_lookup_command(char* translation);

clover_instruction* clover_instruction_from_brackets(
        const char* bracket_contents);

#endif // CLOVER_INSTRUCTION_H
