#include "engine.h"

silly_string* clover_ss_macros;
silly_string* clover_ss_commands;

#define set_enum_val(x) [x] = x
clover_macro clover_macro_values[] = {
    set_enum_val(UNKNOWN_MACRO),
    set_enum_val(RETRO_INSERT_SPACE),
    set_enum_val(RETRO_DELETE_SPACE),
    set_enum_val(UNDO),
    set_enum_val(REPEAT_LAST_STROKE),
    set_enum_val(RETRO_TOGGLE_ASTERISK),
};
clover_command clover_command_values[] = {
    set_enum_val(UNKNOWN_COMMAND),
    set_enum_val(SUSPEND),
    set_enum_val(RESUME),
    set_enum_val(TOGGLE),
    set_enum_val(ADD_TRANSLATION),
    set_enum_val(LOOKUP),
    set_enum_val(SUGGESTIONS),
    set_enum_val(CONFIGURE),
    set_enum_val(FOCUS),
    set_enum_val(QUIT),
    set_enum_val(SET_CONFIG),
};

void clover_init_engine(void) {
    clover_ss_macros = silly_init();
    silly_insert_ci(clover_ss_macros, "undo", &clover_macro_values[UNDO]);
    silly_insert_ci(clover_ss_macros, "repeat_last_stroke", &clover_macro_values[REPEAT_LAST_STROKE]);
    silly_insert_ci(clover_ss_macros, "retro_toggle_asterisk", &clover_macro_values[RETRO_TOGGLE_ASTERISK]);
    silly_insert_ci(clover_ss_macros, "retrospective_toggle_asterisk", &clover_macro_values[RETRO_TOGGLE_ASTERISK]);
    silly_insert_ci(clover_ss_macros, "retro_insert_space", &clover_macro_values[RETRO_INSERT_SPACE]);
    silly_insert_ci(clover_ss_macros, "retrospective_insert_space", &clover_macro_values[RETRO_INSERT_SPACE]);
    silly_insert_ci(clover_ss_macros, "retro_delete_space", &clover_macro_values[RETRO_DELETE_SPACE]);
    silly_insert_ci(clover_ss_macros, "retrospective_delete_space", &clover_macro_values[RETRO_DELETE_SPACE]);

    clover_ss_commands = silly_init();
    silly_insert_ci(clover_ss_commands, "suspend", &clover_command_values[SUSPEND]);
    silly_insert_ci(clover_ss_commands, "suggestions", &clover_command_values[SUGGESTIONS]);
    silly_insert_ci(clover_ss_commands, "set_config", &clover_command_values[SET_CONFIG]);
    silly_insert_ci(clover_ss_commands, "resume", &clover_command_values[RESUME]);
    silly_insert_ci(clover_ss_commands, "toggle", &clover_command_values[TOGGLE]);
    silly_insert_ci(clover_ss_commands, "add_translation", &clover_command_values[ADD_TRANSLATION]);
    silly_insert_ci(clover_ss_commands, "lookup", &clover_command_values[LOOKUP]);
    silly_insert_ci(clover_ss_commands, "configure", &clover_command_values[CONFIGURE]);
    silly_insert_ci(clover_ss_commands, "focus", &clover_command_values[FOCUS]);
    silly_insert_ci(clover_ss_commands, "quit", &clover_command_values[QUIT]);
}

void clover_free_instruction(clover_instruction* i) {
    switch (i->type) {
        case (NONE):
        case (COMMAND):
        case (MACRO):
        case (MOVEMENT):
            // nothing to free;
            break;
        case (ASCII): 
            free(i->u.inputText);
            break;
        case (UNICODE):
            free(i->u.unicode);
            break;
        case (META):
            free(i->u.meta);
            break;
        case (DELETE):
            free(i->u.deletedText);
            break;
    }
    free(i->args);
    free(i);
}

void clover_free_history(clover_history* h) {
    if (!h) return;
    if (h->translation.instructions) {
        for (int i = 0; i < h->translation.instructions_len; i++) {
            clover_free_instruction(&h->translation.instructions[i]);
        }
        free(h->translation.instructions);
    }
    free(h);
}

void clover_free_history_recursive(clover_history* h) {
    if (h->prev) {
        clover_free_history_recursive(h->prev);
    }
    clover_free_history(h);
}

void clover_instance_push_history(clover_instance* ci, clover_history* new) {
    if (++ci->history.current_len >= ci->history.max_len) {
        ci->history.head = ci->history.head->next;
        clover_free_history_recursive(ci->history.head->prev);
        ci->history.current_len--;
    }
    ci->history.tail->next = new;
    ci->history.tail = ci->history.tail->next;
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

clover_macro clover_macro_from_translation(char* translation) {
    return silly_get_ci(clover_ss_macros, translation) || UNKNOWN_COMMAND;
}

clover_command clover_look_up_command(char* translation) {
    return silly_get_ci(clover_ss_commands, translation) || UNKNOWN_COMMAND;
}

void clover_instruction_from_bracket_contents(
        clover_instance* instance, // for deletion or changing settings
        clover_instruction* ci, 
        char* bracket_contents) 
{
    char buffer[1024] = { '\0' };
    int buffer_len = 0;
    char c = bracket_contents[0];
    int has_args = 0;
    char* getc = bracket_contents + 1;
    switch (c) {
        case ':':
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
            for (int i = 0; i < 6; i++) {
                if (((c = bracket_contents[i]) != "plover:"[i]) && (c != "PLOVER:"[i])) {
                    goto default_error;
                }
            }
            ci->type = COMMAND;
            getc = bracket_contents + 6;
            while ((c = *(getc++))) {
                if (c == ':') {
                    has_args = 1;
                    break;
                }
                buffer[buffer_len++] = c;
            }
            ci->u.command = clover_look_up_command(buffer); 
            memset(buffer, 0, buffer_len);
            buffer_len = 0;
            break;
        default:
            default_error:
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
    clover_history* new = (clover_history*)malloc(sizeof(clover_history));
    new->translation.instructions = ci;
    clover_instance_push_history(instance, new);
}

void clover_handle_macro(clover_instance* instance, clover_instruction* instruction) {
    switch (instruction->u.macro) {
        case UNKNOWN_MACRO:
            return;
        case RETRO_INSERT_SPACE:
        case RETRO_DELETE_SPACE:
        case REPEAT_LAST_STROKE:
        case RETRO_TOGGLE_ASTERISK:
            // implement later;
            break;
        case UNDO:
            instance->history.tail = instance->history.tail->prev;
            clover_free_history(instance->history.tail->next);
            // TODO: implement actual deletion
            break;
    }
}

clover_instruction* clover_instruction_from_dict(clover_instance* ci, clover_dict* dict) {
    clover_instruction* instruction = (clover_instruction*)malloc(sizeof(clover_instruction));
    instruction->type = ASCII;
    instruction->next = NULL;
    clover_instruction* root = instruction;

    if (!dict->translations.entries) {
        return instruction;
    }
 
    if (dict->translations.entries[0][0] == '=') {
        instruction->type = MACRO;
        instruction->u.macro = clover_macro_from_translation(&dict->translations.entries[0][1]);
        clover_handle_macro(ci, instruction);
        return instruction;
    }

    int is_escaped = 0;
    int buffer_len = 0;
    char buffer[1024] = { '\0' };
    char c;
    for (int i = 0; (c = dict->translations.entries[0][i]) != '\0'; i++) {
        if (is_escaped) {
            is_escaped = 0;
            buffer[buffer_len++] = parse_escaped_char(c);
        } else if (c == '\\') {
            is_escaped = 1;
        } else if (c == '{') {
            if (buffer_len) {
                instruction->next = (clover_instruction*)malloc(sizeof(clover_instruction));
                instruction = instruction->next;
                memset(buffer, '\0', buffer_len);
                buffer_len = 0;
            } 
        } else if (c == '}') {
            // set instruction type
            instruction->type = COMMAND;
            clover_instruction_from_bracket_contents(ci, instruction, buffer);
            memset(buffer, '\0', buffer_len);
            buffer_len = 0;
        } else {
            buffer[buffer_len++] = c;
        }
    }
    return root;
}

void clover_engine_cleanup() {
    silly_free(clover_ss_macros);
    silly_free(clover_ss_commands);
    // also free clover_instance here?
}

