#include "./clvrlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

dictmap* init_dictmap(int capacity) {
    if (!capacity || capacity % 2) {
        printf("disallowed capacity value");
        exit(1);
    }

    dictmap* d = (dictmap*)malloc(sizeof(dictmap));
    d->size = 0;
    d->capacity = capacity;
    d->dicts = (dict**)calloc(capacity, sizeof(dict*));
    return d;
}

dict* init_dict(unsigned int id, char* translation, dict* parent) {
    dict* d = (dict*)malloc(sizeof(dict));
    d->id = id;
    d->translation = translation;
    d->parent = parent;
    d->children = init_dictmap(8);
    return d;
}

int hash_function(dictmap* m, unsigned int id) {
    //int trailing = __builtin_ctz(m.capacity);
    int hash = 0;
    for (int trailing = __builtin_ctz(m->capacity); id; id >>= trailing) {
        hash ^= id;
    }
    /*for (int i = 0; i < STENO_ORDER_SIZE; i += trailing) {
        hash ^= (id >> i);
    }*/

    return hash % m->capacity;
}

int linear_probe_null(dictmap* m, int start) {
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


int hash_index_of(dictmap* m, unsigned int id) {
    //printf("In hash_index_of(). Map capacity: %i\n", m->capacity);
    int index = hash_function(m, id);
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

int has(dictmap* m, unsigned int id) {
    if (hash_index_of(m, id) + 1) {
        return 1;
    }
    return 0;
}

dict* get(dictmap* m, unsigned int id) {
    int index = hash_index_of(m, id);
    if (index > -1) {
        return m->dicts[index];
    }
    return NULL;
}

// TODO: if an element already exists with a null definition,
// this function adds the definition but none of the children that it 
// maybe should add? idk hashmaps are hard okay
dictmap* add(dictmap* m, dict* element) {
    if (m->capacity >> 1 <= ++m->size) {
        m = scale_up_dictmap(m);
    }

    dict* exists = get(m, element->id);
    
    if (exists) {
        if (exists->translation) {
            printf("collision error: %s\n", pretty_chord(element->id));
            printf(
                "Translation old: %s\nTranslation new: %s\n",
                get(m, element->id)->translation,
                element->translation
            );
            //exit(1);
        } else {
            exists->translation = element->translation;
        }
    } else {
        int index = hash_function(m, element->id);
        index = linear_probe_null(m, index);

        m->dicts[index] = element;
    }

    return m;
}

dictmap* scale_up_dictmap(dictmap* old) {
    dictmap* new = init_dictmap(old->capacity << 1);
    
    for (int i = 0; i < old->capacity; i++) {
        if (!old->dicts[i]) {
            continue;
        }
        new = add(new, old->dicts[i]);
    }

    return new;
}

dict* push_entry(dict* d, uint_arr id, int id_pos, char *translation) {
    dictmap* m = d->children;
    if (id_pos == id.size - 1) {
        d->children = add(m, init_dict(id.arr[id_pos], translation, d));
        return d;
    }
    int target = hash_index_of(m, id.arr[id_pos]);
    if (target > -1) {
        m->dicts[target] = push_entry(
            m->dicts[target], id, id_pos + 1, translation
        );
    } else {
        dict* new_entry = init_dict(id.arr[id_pos], NULL, d);
        new_entry = push_entry(new_entry, id, id_pos + 1, translation);
        m = add(m, new_entry);
    }
    d->children = m;
    return d;
}
