#include "dict.h"

void clover_free_dict(clover_dict* d) {
    for (int i = 0; i < d->children.capacity; i++) {
        if (d->children.dicts[i]) {
            clover_free_dict(d->children.dicts[i]);
        }
    }
    free(d->children.dicts);
    for (int i = 0; i < d->translations.len; i++) {
        free(d->translations.entries[i]);
    }
    free(d->translations.entries);
    free(d);
}

clover_dict* clover_table_init_dict(clover_chord id, clover_dict* parent) {
    clover_dict* d = (clover_dict*)malloc(sizeof(clover_dict));
    d->id = id;
    d->translations.len = 0;
    d->translations.entries = NULL;
    d->parent = parent;
    d->children.size = 0;
    d->children.capacity = 8;
    d->children.dicts = (clover_dict**)calloc(d->children.capacity, sizeof(clover_dict*));
    return d;
}

int clover_table_dict_add_translation(clover_dict* d, char* trns) {
    d->translations.len++;
    d->translations.entries = (char**)realloc(d->translations.entries, d->translations.len * sizeof(char*));
    d->translations.entries[d->translations.len - 1] = (char*)malloc(sizeof(char) * (strlen(trns) + 1));
    strcpy(d->translations.entries[d->translations.len - 1], trns);
    return d->translations.len;
}

int clover__table_hash_function(clover_dict* d, clover_chord id) {
    unsigned int hash = 0;
    for (int trailing = __builtin_ctz(d->children.capacity); id; id >>= trailing) {
        hash ^= id;
    }
    return (int)(hash % d->children.capacity);
}

// ONLY for inserting children with unique ID's. 
// doesn't check other id's during linear probe
clover_dict* clover__dict_insert_child(clover_dict* parent, clover_dict* child) {
    int index = clover__table_hash_function(parent, child->id);
    int max_index = parent->children.capacity;
    
linear_probe_null:
    for (int i = index; i < max_index; i++) {
        if (!parent->children.dicts[i]) {
            return (parent->children.dicts[i] = child);
        }
    }
    max_index = index;
    if (index) {
        index = 0;
        goto linear_probe_null;
    } else {
        printf("clover_dict_insert_child error\n");
        exit(1);
    }
}

void clover__dict_resize(clover_dict* parent) {
    clover_dict temp;
    temp.children.size = parent->children.size;
    temp.children.capacity = parent->children.capacity << 1;
    temp.children.dicts = (clover_dict**)calloc(temp.children.capacity, sizeof(clover_dict*));

    clover_dict* child;

    for (int i = 0; i < parent->children.capacity; i++) {
        if ((child = parent->children.dicts[i]) != NULL) {
            clover__dict_insert_child(&temp, child);
        }
    }
    free(parent->children.dicts);
    parent->children = temp.children;
}

clover_dict* clover__dict_get(clover_dict* parent, clover_chord target_id, int create) {
    int index = clover__table_hash_function(parent, target_id);
    int max_index = parent->children.capacity;
    clover_dict* current_dict;

linear_probe_get:
    for (int i = index; i < max_index; i++) {
        if ((current_dict = parent->children.dicts[i]) != NULL) {
            if (current_dict->id == target_id) {
                return current_dict;
            }
        } else {
            if (create) {
                current_dict = clover_table_init_dict(target_id, parent);
                if (parent->children.capacity >> 1 <= ++parent->children.size) {
                    clover__dict_resize(parent);
                }
                return clover__dict_insert_child(parent, current_dict);
            }
            return NULL;
        }
    }
    max_index = index;
    if (index) {
        index = 0;
        goto linear_probe_get;
    } else {
        printf("clover_get error\n");
        exit(1);
    }
}

clover_dict* clover_dict_get(clover_dict* parent, clover_chord target_id) {
    return clover__dict_get(parent, target_id, 0);
}

clover_dict* clover_dict_push_entry(clover_dict* d, clover_chord* id, int id_size, char* translation) {
    for (int i = 0; i < id_size; i++) {
        d = clover__dict_get(d, id[i], 1);
    }
    clover_table_dict_add_translation(d, translation);
    return d;
}

clover_dict* clover_seek(clover_dict* d, clover_chord* id, int id_len) {
    for (int i = 0; (i < id_len) && (d = clover__dict_get(d, id[i], 0)); i++) {}
    return d;
}