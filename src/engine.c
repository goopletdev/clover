#include "engine.h"

void clover_free_history(clover_history* h) {
    if (!h) return;
    free(h->translation.inputText);
    free(h->translation.deletedText);
    clover_free_history(h->previous);
    free(h);
}

void clover_instance_push_history(clover_instance* ci, clover_history new) {
    if (++ci->history.current_size >= ci->history.max_size) {
        ci->history.first = ci->history.first->next;
        clover_free_history(ci->history.first->previous);
        ci->history.current_size--;
    }
    // figure out how to implement this after you sleep a little
}