#include "history.h"

clover_history* clover_history_init(void) {
    clover_history* cl_hist = (clover_history*)
        malloc(sizeof(clover_history));
    cl_hist->len = 0;
    cl_hist->head = NULL;
    cl_hist->tail = NULL;
    return cl_hist;
}

clover_history_element* clover_history_element_init(void) {
    clover_history_element* cl_el = (clover_history_element*)
        malloc(sizeof(clover_history_element));
    cl_el->instruction = NULL;
    cl_el->source_dict = NULL;
    cl_el->next = NULL;
    cl_el->prev = NULL;
    return cl_el;
}

void clover_history_push(
        clover_history* history, clover_history_element* element) {
    if (history->len++ == 0) {
        history->tail = element;
        history->head = element;
    } else {
        element->prev = history->tail;
        history->tail->next = element;
        history->tail = element;
    }
}

clover_history_element* clover_history_pop(clover_history* history) {
    clover_history_element* el = history->tail;
    history->tail = el->prev;
    if (history->tail) {
        history->tail->next = NULL;
    }
    el->prev = NULL;
    history->len--;
    return el;
}

clover_history_element* clover_history_shift(clover_history* history) {
    clover_history_element* el = history->head;
    history->head = el->next;
    if (history->head) {
        history->head->prev = NULL;
    }
    el->next = NULL;
    history->len--;
    return el;
}

void clover_history_free_element(clover_history_element* el) {
    if (el) {
        free(el->instruction);
    }
    free(el);
}

void clover_history_free(clover_history* history) {
    while (history->tail) {
        clover_history_free_element(clover_history_pop(history));
    }
    free(history);
}

