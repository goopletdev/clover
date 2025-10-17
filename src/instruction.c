#include "instruction.h"

silly_string* clover_macros_trie;
silly_string* clover_commands_trie;

#define set_i(i) [i] = i
clover_macro clover_macro_values[] = {
    set_i(UNKNOWN_MACRO),
    set_i(RETRO_INSERT_SPACE),
    set_i(RETRO_DELETE_SPACE),
    set_i(UNDO),
    set_i(REPEAT_LAST_STROKE),
    set_i(RETRO_TOGGLE_ASTERISK),
};
clover_command clover_command_values[] = {
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
    clover_commands_trie = silly_init();
    #define silly_macro(x) silly_insert_ci(clover_macros_trie, #x, &clover_macro_values[x])
    silly_macro(UNDO);
    silly_macro(REPEAT_LAST_STROKE);
    silly_macro(RETRO_TOGGLE_ASTERISK);
    silly_insert_ci(clover_macros_trie, "retrospective_toggle_asterisk", &clover_macro_values[RETRO_TOGGLE_ASTERISK]);
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

void clover_instruction_cleanup(void) {
    silly_free(clover_macros_trie);
    silly_free(clover_commands_trie);
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

// internal fucntion
char clover_instruction__parse_escaped_char(char c) {
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

clover_macro clover_instruction_lookup_macro(char* translation) {
    const clover_macro* m = silly_get_ci(clover_macros_trie, translation);
    return m ? *m : UNKNOWN_MACRO; 
}

clover_command clover_instruction_lookup_command(char* translation) {
    const clover_command* c = silly_get_ci(clover_commands_trie, translation);
    return c ? *c : UNKNOWN_COMMAND;
}

clover_instruction* clover_instruction_from_brackets(
        const char* bracket_contents)
{
    char buffer[1024] = { '\0' };
    int buffer_len = 0;
    char c = bracket_contents[0];
    int has_args = 0;
    const char* getc = bracket_contents + 1;
    clover_instruction* ci = (clover_instruction*)malloc(sizeof(clover_instruction));
    /*
     * temporary, before bracket commands are implemented:
     */
    printf("bracket contents: [%s]\n", bracket_contents);
    ci->type = ASCII;
    ci->u.inputText = (char*)malloc(sizeof(char) * (strlen(bracket_contents) + 1));
    strcpy(ci->u.inputText, bracket_contents);
    return ci;
    // end temporary pre-implementation hack thing

    printf("%c\n", c);
    switch (c) {
        case ':':
            if (*getc == '\0') {
                goto colon_punctuation;
            }
            // instruction type is 'meta'
            while ((c = *(getc++))) {
                if (c == ':') {
                    has_args = 1;
                    break;
                }
                buffer[buffer_len++] = c;
            }
            ci->type = META;
            ci->u.meta = (char*)malloc(sizeof(char) * buffer_len);
            strcpy(ci->u.meta, buffer);
            memset(buffer, 0, buffer_len);
            buffer_len = 0;
            break;
        case 'p':
        case 'P':
            // instruction type should be 'command'
            break;
        case 'm':
        case 'M':
            // instruction type should be  'mode'
            break;
        case '^':
        case '&':
        case '*':
        case '-':
        case '>':
        case '<':
        case '~':
        case '.':
        case ',':
        case '?':
        case '!':
        // case ':':
        colon_punctuation:
        case ';':
        case '=':
        case '#':
        case '$':
            // implement all of these later
            break;
        default: 
            printf("Dictionary error: unrecognized command [%s]\n", bracket_contents);
            exit(1);
    }
    if (has_args) {
        while ((c = *(getc++))) {
            buffer[buffer_len++] = c;
        }
        ci->args = (char*)malloc(buffer_len * sizeof(char));

        strcpy(ci->args, buffer);
        memset(buffer, 0, buffer_len);
        buffer_len = 0;
    }
    return ci;
}

clover_instruction* clover_instruction_from_macro(
        const char* macro_contents)
{
    // macros start with '='. this function takes everything following '='
    char buffer[1023] = { '\0' };
    int buffer_len = 0;
    char c;
    const char* getc = macro_contents;
    clover_instruction* ci = (clover_instruction*)malloc(sizeof(clover_instruction));
    ci->type = MACRO;
    while ((c = *(getc++)) && c != ':') {
        buffer[buffer_len++] = c;
    }
    ci->u.macro = clover_instruction_lookup_macro(buffer);
    if (c) {
        // has args
        ci->args = (char*)malloc(strlen(getc) * sizeof(char));
        strcpy(ci->args, getc);
    }
    return ci;
}

clover_instruction* clover_instruction__add_space(clover_instruction* inst, int before) {
    clover_instruction* space = (clover_instruction*)malloc(sizeof(clover_instruction));
    space->type = ASCII;
    space->u.inputText = (char*)malloc(sizeof(char) * 2);
    space->u.inputText[0] = ' ';
    space->u.inputText[1] = '\0';

    if (before) {
        space->next = inst;
        space->prev = NULL;
        if (inst) {
            inst->prev = space;
        }
        return space;
    } else {
        // seek last instruction
        clover_instruction* seek = inst;
        while (seek->next) { seek = seek->next; }
        inst->next = space;
        space->prev = inst;
        space->next = NULL;
        return inst;
    }
}

clover_instruction* clover_instruction_from_dict(clover_dict* dict, clover_chord chord) {
    clover_instruction* inst = NULL;
    clover_instruction* root = NULL;
    if (!dict || !dict->translations.entries) {
        // sending just the literal stroke instead of a translation
        root = (clover_instruction*)malloc(sizeof(clover_instruction));
        root->type = ASCII;
        root->u.inputText = clover_pretty_chord(chord);
        root->next = NULL;
        root->prev = NULL;
        return clover_instruction__add_space(root, 1);
    }

    if (dict->translations.entries[0][0] == '=') {
        root = clover_instruction_from_macro(dict->translations.entries[0] + 1);
        root->next = NULL;
        root->prev = NULL;
        return root;
    }

    int is_escaped = 0;
    int buffer_len = 0;
    char buffer[1024] = { '\0' };
    char c;
    char* getc = dict->translations.entries[0];
    while ((c = *(getc++))) {
        if (is_escaped) {
            is_escaped = 0;
            buffer[buffer_len++] = clover_instruction__parse_escaped_char(c);
        } else if (c == '\\') {
            is_escaped = 1;
        } else if (c == '{') {
            if (!inst) {
                inst = (clover_instruction*)malloc(sizeof(clover_instruction));
                root = inst;
            }
            if (buffer_len) {
                inst->type = ASCII;
                inst->u.inputText = (char*)malloc(sizeof(char) * buffer_len);
                strcpy(inst->u.inputText, buffer);
                inst->next = NULL;
                memset(buffer, '\0', buffer_len);
            }
        } else if (c == '}') {
            inst->next = clover_instruction_from_brackets(buffer);
            inst = inst->next;
            inst->next = NULL;
            memset(buffer, '\0', buffer_len);
            buffer_len = 0;
        } else {
            buffer[buffer_len++] = c;
        }
    }
    if (buffer_len) {
        if (!inst) {
            inst = (clover_instruction*)malloc(sizeof(clover_instruction));
            root = inst;
        } else {
            inst->next = (clover_instruction*)malloc(sizeof(clover_instruction));
            inst = inst->next;
        }
        inst->type = ASCII;
        inst->u.inputText = (char*)malloc(sizeof(char) * buffer_len);
        strcpy(inst->u.inputText, buffer);
        inst->next = NULL;
    }
    return clover_instruction__add_space(root, 1);
}
