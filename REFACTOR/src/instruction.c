#include "instruction.h"

#define CLV_GDT_INSERT(x) gd_trie_insert_ci(t, #x, (void*)x)

gd_trie* clover_instruction_init_macros(void) {
    gd_trie* t = gd_trie_init();
    CLV_GDT_INSERT(RETRO_INSERT_SPACE);
    CLV_GDT_INSERT(RETRO_DELETE_SPACE);
    CLV_GDT_INSERT(UNDO);
    CLV_GDT_INSERT(REPEAT_LAST_STROKE);
    CLV_GDT_INSERT(RETRO_TOGGLE_ASTERISK);
    gd_trie_insert_ci(
            t, "RETROSPECTIVE_TOGGLE_ASTERISK", (void*)RETRO_TOGGLE_ASTERISK);
    gd_trie_insert_ci(
            t, "RETROSPECTIVE_INSERT_SPACE", (void*)RETRO_INSERT_SPACE);
    gd_trie_insert_ci(
            t, "RETROSPECTIVE_DELETE_SPACE", (void*)RETRO_DELETE_SPACE);
    return t;
}

gd_trie* clover_instruction_init_commands(void) {
    gd_trie* t = gd_trie_init();
    CLV_GDT_INSERT(SUSPEND);
    CLV_GDT_INSERT(RESUME);
    CLV_GDT_INSERT(TOGGLE);
    CLV_GDT_INSERT(ADD_TRANSLATION);
    CLV_GDT_INSERT(LOOKUP);
    CLV_GDT_INSERT(SUGGESTIONS);
    CLV_GDT_INSERT(CONFIGURE);
    CLV_GDT_INSERT(FOCUS);
    CLV_GDT_INSERT(QUIT);
    CLV_GDT_INSERT(SET_CONFIG);
    return t;
}

clover_instruction_free_node(clover_instruction_node* inst) {
    if (!inst) {
        return;
    }
    switch (inst->type) {
        case (NONE):
        case (MACRO):
        case (COMMAND):
            break;
        case (STRING):
            free(inst->u.string);
            break;
        case (META):
            free(inst->u.meta);
            break;
        case (DELETE):
            free(inst->u.delete);
            break;
    }
    free(inst->args);
    free(inst);
}

clover_instruction_free(clover_instruction* inst) {
    gd_dll_free(inst, clover_instruction_free_node);
}

char clover__instruction_parse_escaped_char(char c) {
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

inline int clover__instruction_trie_lookup(gd_trie* t, char* translation) {
    return (int)(uintptr_t)gd_trie_get_ci(t, translation);
}

clover_instruction_node* clover_instruction_from_brackets(
        const char* bracket_contents)
{
    char char_buffer[512] = { '\0' };
    int buffer_len = 0;
    char c = bracket_contents[0];
    int has_args = 0;
    const char* getc = bracket_contents + 1;
    clover_instruction_node* ci = (clover_instruction_node*)malloc(sizeof(clover_instruction_node));
    // TEMPORARY, before commands are implemented:
    int bracket_contents_len = strlen(bracket_contents):
    ci->type = STRING;
    ci->u.string = (char*)malloc(sizeof(char) * bracket_contents_len + 3);
    ci->u.string[0] = '{';
    strcpy(ci->u.string + 1, bracket_contents);
    ci->u.string[bracket_contents_len + 1] = '}';
    ci->u.string[bracket_contents_len + 2] = '\0';
    strcat(ci->u.string, "}");
    return ci;
    // end temporary pre-implementation hack thing
}

clover_instruction_node* clover_instruction_from_macro(
        const char* macro_contents, gd_trie* macro_trie)
{
    char buffer[1024] = { '\0'};
    int buffer_len = 0;
    char c;
    const char* getc = macro_contents;
    clover_instruction_node* ci = (clover_instruction_node*)malloc(sizeof(clover_instruction_node));
    ci->type = MACRO;
    while ((c = *(getc++)) && c != ':') {
        buffer[buffer_len++] = c;
    }
    ci->u.macro = clover__instruction_trie_lookup(macro_trie, buffer);
    if (c) {
        ci->args = (char*)malloc(strlen(getc) * sizeof(char));
        strcpy(ci->args, getc);
    } else {
        ci->args = NULL;
    }
    return ci;
}

void clover__instruction_space(
        clover_instruction i, struct clover_settings* cs)
{
    clover_instruction_node* space = (clover_instruction*)malloc(sizeof(clover_instruction));
    space->type = SPACE;
    space->u.string = (char*)malloc(2);
    strncpy(space->u.string, 2, cs->space_mode);
    if (cs->space_before) {
        gd_dll_unshift(i, space);
    } else {
        gd_dll_push(i, space);
    }
}

clover_instruction* clover_instruction_from_dict(
        clover_dict* dict, clover_history* h, struct clover_settings* cs)
{
    clover_instruction* inst = gd_dll_init();
    clover_instruction_node* node = NULL;
    if (!dict || !dict->translations.entries) {
        // send literal stroke instead of translation
        node = (clover_instruction_node*)malloc(sizeof(clover_instruction_node));
        node->type = STRING;
        node->u.string = (char*)malloc(clover_chord_size(chord) + 1);
        clover_pretty_chord(node->u.string, 512, chord);
        gd_dll_push(inst, node);
        clover__instruction_space(inst, cs);
        return inst;
    }

    if (dict->translations.entries[0][0] == '=') {
        node = clover_instruction_from_macro(dict->translations.entries[0] + 1);
        gd_dll_push(inst, node);
        return inst;
    }

    int is_escaped = 0;
    int buffer_len = 0;
    char char_buffer[1024] = { '\0' };
    char c;
    char* getc = dict->translations.entries[0];
    while ((c = *(getc++))) {
        if (is_escaped) {
            is_escaped = 0;
            char_buffer[buffer_len++] = clover__instruction_parse_escaped_char(c);
        } else if (c == '\\') {
            is_escaped = 1;
        } else if (c == '{') {
            if (buffer_len) {
                node = (clover_instruction_node*)malloc(sizeof(clover_instruction_node));
                node->type = STRING;
                node->u.string = (char*)malloc(sizeof(char) * buffer_len);
                strcpy(inst->u.string, char_buffer);
                gd_dll_push(inst, node);
                memset(char_buffer, '\0', buffer_len);
                node = NULL;
            }
        } else if (c == '}') {
            node = clover_instruction_from_brackets(char_buffer);
            gd_dll_push(inst, node);
            memset(char_buffer, '\0', buffer_len);
            buffer_len = 0;
            node = NULL;
        } else {
            buffer[buffer_len++] = c;
        }
    }
    if (buffer_len) {
        node = (clover_instruction_node*)malloc(sizeof(clover_instruction_node));
        node->type = STRING;
        node->u.string = (char*)malloc(sizeof(char) * buffer_len);
        strcpy(node->u.string, char_buffer);
        gd_dll_push(inst, node);
    }
    clover__instruction(inst, cs);
    return inst;
}

