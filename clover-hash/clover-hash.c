#include "./clover-hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct clover__dictT {
    clover_chord id;
    char* translation;
    struct clover__dictT* parent;
    struct clover__dictmapT* children;
};

struct clover__dictmapT {
    int size;
    int capacity;
    struct clover__dictT** dicts;
};

typedef struct clover__dictmapT clover__dictmap;
clover__dictmap* clover__scale_up_dictmap(clover__dictmap* old);


// ##################
// #    getters:    #
// ##################

int clover_dict_size(clover_dict* d) {
    return d->children->size;
}

clover_chord clover_dict_id(clover_dict* d) {
    return d->id;
}

const char* clover_dict_translation(clover_dict* d) {
    return d->translation;
}

const clover_dict* clover_dict_parent(clover_dict* d) {
    return d->parent;
}

// ##################
// #  /getters end  #
// ##################

void clover_dict_children_foreach(clover_dict* d, int (*func)(clover_dict*)) {
    for (int i = 0; i < d->children->capacity; i++) {
        if (d->children->dicts[i]) {
            func(d->children->dicts[i]);
        }
    }
}

clover__dictmap* clover__init_dictmap(int capacity) {
    if (!capacity || capacity % 2) {
        printf("disallowed capacity value");
        exit(1);
    }

    clover__dictmap* d = (clover__dictmap*)malloc(sizeof(clover__dictmap));
    d->size = 0;
    d->capacity = capacity;
    d->dicts = (clover_dict**)calloc(capacity, sizeof(clover_dict*));
    return d;
}

clover_dict* clover_init_dict(
    clover_chord id, char* translation, clover_dict* parent
) {
    clover_dict* d = (clover_dict*)malloc(sizeof(clover_dict));
    d->id = id;
    d->translation = translation;
    d->parent = parent;
    d->children = clover__init_dictmap(8);
    return d;
}

int clover__hash_function(clover__dictmap* m, clover_chord id) {
    unsigned int hash = 0;
    for (int trailing = __builtin_ctz(m->capacity); id; id >>= trailing) {
        hash ^= id;
    }
    return (int)(hash % m->capacity);
}

int clover__linear_probe_null(clover__dictmap* m, int start) {
    int i;
    for (i = start; i < m->capacity; i++) {
        if (!m->dicts[i]) {
            return i;
        }
    }
    for (i = 0; i < start; i++) {
        if (!m->dicts[i]) {
            return i;
        }
    }
    return -1;
}

int clover__hash_index_of(clover__dictmap* m, clover_chord id) {
    int index = clover__hash_function(m, id);
    for (int i = index; i < m->capacity; i++) {
        if (m->dicts[i]) {
            if (m->dicts[i]->id == id) {
                return i;
            }
        } else {
            return -1;
        }
    }
    for (int i = 0; i < index; i++) {
        if (m->dicts[i]) {
            if (m->dicts[i]->id == id) {
                return i;
            }
        } else {
            return -1;
        }
    }
    return -1;
}

int clover_has(clover_dict* d, clover_chord id) {
    clover__dictmap* m = d->children;
    if (clover__hash_index_of(m, id) + 1) {
        return 1;
    }
    return 0;
}

clover_dict* clover__get_from_map(clover__dictmap* m, clover_chord id) {
    int index = clover__hash_index_of(m, id);
    if (index > -1) {
        return m->dicts[index];
    }
    return NULL;
}

clover_dict* clover_get(clover_dict* d, clover_chord id) {
    return clover__get_from_map(d->children, id);
}

// TODO: if an element already exists with a null definition,
// this function adds the definition but none of the children that it 
// maybe should add? idk hashmaps are hard okay
clover__dictmap* clover__add(clover__dictmap* m, clover_dict* element) {
    if (m->capacity >> 1 <= ++m->size) {
        m = clover__scale_up_dictmap(m);
        m->size++;
    }

    clover_dict* exists = clover__get_from_map(m, element->id);
    
    if (exists) {
        if (exists->translation) {
            printf("collision error: %s\n", clover_pretty_chord(element->id));
            printf(
                "Translation old: %s\nTranslation new: %s\n\n",
                clover__get_from_map(m, element->id)->translation,
                element->translation
            );
            //exit(1);
        } else {
            exists->translation = element->translation;
        }
    } else {
        int index = clover__hash_function(m, element->id);
        index = clover__linear_probe_null(m, index);

        m->dicts[index] = element;
    }

    return m;
}

clover__dictmap* clover__scale_up_dictmap(clover__dictmap* old) {
    clover__dictmap* new = clover__init_dictmap(old->capacity << 1);
    
    for (int i = 0; i < old->capacity; i++) {
        if (!old->dicts[i]) {
            continue;
        }
        new = clover__add(new, old->dicts[i]);
    }

    return new;
}

clover_dict* clover_push_entry(clover_dict* d,
    clover_chord* id,
    int id_size,
    const char *translation
) {
    clover_dict* target_dict = clover__get_from_map(d->children, id[0]);
    if (!target_dict) {
        target_dict = clover_init_dict(id[0], NULL, d);
        d->children = clover__add(d->children, target_dict);
    }

    if (id_size == 1) {
        target_dict->translation = (char*)malloc(
            (strlen(translation) + 1) * sizeof(char)
        );
        strcpy(target_dict->translation, translation);
    } else {
        clover_push_entry(target_dict, &id[1], id_size - 1, translation);
    }

    return d;
}
