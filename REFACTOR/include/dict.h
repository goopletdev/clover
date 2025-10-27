#ifndef CLOVER_DICT_H
#define CLOVER_DICT_H

#include "gd-ordered-map.h"
#include "chord.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct clover__dictT clover_dict;
struct clover__dictT {
    clover_chord id;
    gd_dll* translations;
    clover_dict* parent;
    gd_omap* children;
};

void clover_dict_free(void* d);
clover_dict* clover_dict_init(clover_chord id, clover_dict* parent);
int clover_dict_push_translation(clover_dict* d, char* trns);
clover_dict* clover_dict_get(clover_dict* parent, clover_chord target_id);
clover_dict* clover_dict_add_entry(clover_dict* d, clover_chord* id, char* trns);
clover_dict* clover_dict_seek(clover_dict* d, clover_chord* id);

#endif // CLOVER_DICT_H

