#include "instruction.h"

silly_string* clover_macros_trie;
silly_string* clover_commands_trie;

#define set_i(i) [i] = i
const clover_macro clover_macro_values[] = {
    set_i(UNKNOWN_MACRO),
    set_i(RETRO_INSERT_SPACE),
    set_i(RETRO_DELETE_SPACE),
    set_i(UNDO),
    set_i(REPEAT_LAST_STROKE),
    set_i(RETRO_TOGGLE_ASTERISK),
};
const clover_command clover_command_values[] = {
    set_i(UNKNOWN_COMMAND),
    set_i(SUSPEND),
    set_i(RESUME),
    set_i(TOGGLE),
    set_i(ADD_TRANSLATION),
    set_i(LOOKUP),
    set_i(SUGGESTIONS),
    set_i(CONFIGURE),
    set_i(FOCUS),
    set_i(QUIT),
    set_i(SET_CONFIG),
};

void clover_instruction_start(void) {
    clover_macros_trie = silly_init();
#define silly_macro(x) silly_insert_ci(clover_macros_trie, #x, &clover_macro_values[x])
    silly_macro(UNDO);
    silly_macro(REPEAT_LAST_STROKE);
    silly_macro(RETRO_TOGGLE_ASTERISK);
    silly_macro(RETROSPECTIVE_TOGGLE_ASTERISK);
    silly_macro(RETRO_INSERT_SPACE);
    silly_insert_ci(clover_macros_trie, "retrospective_insert_space", &clover_macro_values[RETRO_INSERT_SPACE]);
    silly_macro(RETRO_DELETE_SPACE);
    silly_insert_ci(clover_macros_trie, "retrospective_delete_space", &clover_macro_values[RETRO_DELETE_SPACE]);
#define silly_command(x) silly_insert_ci(clover_commands_trie, #x, &clover_command_values[x])
    silly_command(UNKNOWN_COMMAND);
    silly_command(SUSPEND);
    silly_command(RESUME);
    silly_command(TOGGLE);
    silly_command(ADD_TRANSLATION);
    silly_command(LOOKUP);
    silly_command(SUGGESTIONS);
    silly_command(CONFIGURE);
    silly_command(FOCUS);
    silly_command(QUIT);
    silly_command(SET_CONFIG);
}

void clover_instruction_free(clover_instruction* inst) {
    if (!inst) {
        return;
    }
    switch (inst->type) {
        case (NONE):
        case (COMMAND):
        case (MACRO):
        case(MOVEMENT):
            // all primitives; nothing to free
            break;
        case (ASCII):
            free(inst->u.inputText);
            break;
        case (UNICODE):
            free(inst->u.unicode);
            break;
        case (META):
            free(inst->u.meta);
            break;
        case (DELETE):
            free(inst->u.deletedText);
            break;
    }
    free(inst->args);
    clover_instruction_free(inst->next);
    free(inst);
}

char parse_escaped_char(char c) {
    switch (c) {
        case ('t'):
            return '\t';
        case ('b'):
            return '\b';
        case ('n'):
            return '\n';
        default:
            return c;
    }
}


