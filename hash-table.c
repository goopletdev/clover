#include "./clvrlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

dictmap scale_up_dictmap(dictmap old);

dictmap init_dictmap(int capacity) {
    if (!capacity || capacity % 2) {
        printf("disallowed capacity value");
        exit(1);
    }

    dictmap d;
    d.size = 0;
    d.capacity = capacity;
    d.dicts = (dict**)calloc(capacity, sizeof(dict*));
    return d;
}

dict init_dict(unsigned int id, char* translation, dict* parent) {
    dict d;
    d.id = id;
    d.translation = translation;
    d.parent = parent;
    d.children = init_dictmap(8);
    return d;
}

int hash_function(dictmap m, unsigned int id) {
    int trailing = __builtin_ctz(m.capacity);
    int hash = 0;
    for (int i = 0; i < STENO_ORDER_SIZE; i += trailing) {
        hash ^= (id >> i);
    }

    return (int) hash % m.capacity;
}

int linear_probe_null(dictmap m, int start) {
    int i;
    for (i = start; i < m.capacity; i++) {
        if (!m.dicts[i]) {
            return i;
        }
    }
    for (i = 0; i < start; i++) {
        if (!m.dicts[i]) {
            return i;
        }
    }
    return -1;
}


int hash_index_of(dictmap m, unsigned int id) {
    int index = hash_function(m, id);
    for (int i = index; i < m.capacity; i++) {
        if ((*(m.dicts[i])).id == id) {
            return i;
        }
    }
    for (int i = 0; i < index; i++) {
        if ((*(m.dicts[i])).id == id) {
            return i;
        }
    }
    return -1;
}

int has(dictmap m, unsigned int id) {
    if (hash_index_of(m, id) + 1) {
        return 1;
    }
    return 0;
}

dict* get(dictmap* m, unsigned int id) {
    int index = hash_index_of(m, id);
    if (index > -1) {
        return (*dictmap).dicts[index];
    }
    return NULL;
}

dictmap add(dictmap m, dict element) {
    if (m.capacity >> 1 <= ++m.size) {
        m = scale_up_dictmap(m);
    }
    
    if (has(m, element.id)) {
        printf("collision error\n");
        exit(1);
    }

    int index = hash_function(m, element);
    index = linear_probe_null(m, index);

    m.dicts[index] = (dict*)malloc(sizeof(dict));
    *(m.dicts[index]) = element;
    return m;
}

dictmap scale_up_dictmap(dictmap old) {
    dictmap new = init_dictmap(old.capacity << 1);
    
    for (int i = 0; i < old.capacity; i++) {
        new = add(new, *old.dicts[i]);
    }

    return new;
}

dict push_entry(dict d, u_int_array id, int id_pos, char *trns) {
    dictmap m = d.children;
    if (id_pos == id.size - 1) {
        d.children = add(m, init_dict(id[id_pos], trns, d));
        return d;
    }
    int target = hash_index_of(m, id[id_position]);
    if (target > -1) {
        *m.dicts[target] = push_entry(*m.dicts[target], id, id_pos + 1, trns);
    } else {
        m = add(m, init_dict(id[id_pos], trns, d));
    }
    d.children = m;
    return d;
}
