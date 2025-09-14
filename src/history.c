#include "history.h"

clover_history* clover_history_init(void) {
    clover_history* cl_hist = (clover_history*)malloc(sizeof(clover_history));
    cl_hist->current_len = 0;
    cl_hist->head = NULL;
    cl_hist->tail = NULL;
    return cl_hist;
}

clover_history_element* clover_history_element_init(void) {
    clover_history_element* cl_el = (clover_history_element*)malloc(sizeof(clover_history_element));
    cl_el->instruction = NULL;
    cl_el->source_dict = NULL;
    cl_el->next = NULL;
    cl_el->prev = NULL;
    return cl_el;
}

void clover_history_push(clover_history* history, clover_history_element* element) {
    element->prev = history->tail;
    history->tail->next = element;
    history->tail = element;
    history->current_len++;
}

clover_history_element* clover_history_pop(clover_history* history) {
    clover_history_element* el = history->tail;
    history->tail = el->prev;
    el->prev = NULL;
    history->current_len--;
    return el;
}