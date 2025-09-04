#include "engine.h"

void clover_free_instruction(clover_instruction* i) {
    switch (i->type) {
        case (ASCII): 
            free(i->u.inputText);
        case (UNICODE):
            free(i->u.unicode);
        case (COMMAND):
            free(i->u.keyCombo);
        case (DELETE):
            free(i->u.deletedText);
    }
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

clover_history* clover_history_from_dict(clover_instance* ci, clover_dict* dict) {
    clover_history* new = (clover_history*)malloc(sizeof(clover_history));
    new.translation.instructions = null;
    new.translation.instructions_len = 0;
    new.translation.source_dict = dict;

    clover_instruction* instruction = (clover_history*)malloc(sizeof(clover_instruction));

    int is_escaped = 0;
    int buffer_len = 0;
    char buffer[1024] = { '\0' };
    if (!dict->translations.entries) {
        instruction.type = NONE;
        new.translation.instructions = instruction;
        return new;
    }
    char c;
    for (int i = 0; (c = dict->translations.entries[i]) != '\0'; i++) {
        if (is_escaped) {
            is_escaped = 0;
            buffer[buffer_len++] = parse_escaped_char(c);
        } else if (c == '{') {
            if (buffer_len) {
                instruction


        
    
}
