#include "dict.h"

void clover_dict_free(void* dict) {
    clover_dict* d = (clover_dict*)dict;
    gd_omap_free(d->children, clover_dict_free);
    gd_dll_cleanup(d->translations, free);
    free(d);
}

clover_dict* clover_dict_init(clover_chord id, clover_dict* parent) {
    clover_dict* d = (clover_dict*)malloc(sizeof(clover_dict));
    d->id = id;
    d->translations = gd_dll_init();
    d->parent = parent;
    d->children = gd_omap_init(8);
    return d;
}

int clover_dict_push_translation(clover_dict* d, char* trns) {
    char* str = (char*)malloc(sizeof(char) * (strlen(trns) + 1));
    strcpy(str, trns);
    gd_dll_push(d->translations, str);
    return d->translations->length;
}

clover_dict* clover_dict_get(clover_dict* parent, clover_chord target_id) {
    return (clover_dict*)gd_omap_get(parent->children, target_id);
}

clover_dict* clover_dict_add_entry(clover_dict* d, clover_chord* id, char* trns) {
    clover_dict* current = d;
    clover_dict* next = NULL;
    clover_chord c;
    for (; (c = id[0]); id++) {
        next = gd_omap_get(current->children, c);
        if (!next) {
            next = clover_dict_init(c, current);
            gd_omap_add(current->children, c, next);
        } else {
        }
        current = next;
    }
    clover_dict_push_translation(current, trns);
    return current;
}

clover_dict* clover_dict_seek(clover_dict* d, clover_chord* id) {
    for (clover_chord c; (c = *id) && (d = clover_dict_get(d, c)); id++) {}
    return d;
}


