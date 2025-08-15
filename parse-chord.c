#include "./clvrlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

dictionary init_dict(unsigned int id, char* definition, dictionary* parent) {
    dictionary dict;
    dict.id = id;
    dict.definition = definition;
    dict.parent = parent;
    dict.children = NULL;
    dict.size = 0;
    return dict;
}

/**
 * this function assumes that all entries are being added in proper steno order
 * */
dictionary push_entry(dictionary dict, uint_arr id, int id_position, char *definition) {
    // get last child
    dictionary *last_child;
    if (dict.size <= 0 || ((last_child = &(dict.children[dict.size - 1]))->id != id.arr[id_position])) {
        dict.children = (dictionary*)realloc(dict.children, (++dict.size) * sizeof(dictionary));
        dict.children[dict.size - 1] = init_dict(id.arr[id_position], NULL, &dict);
        last_child = &(dict.children[dict.size - 1]);
    }

    if (id_position == id.size - 1) {
        if (last_child->definition) {
            printf("collision error: %s: %s\n", pretty_chord(last_child->id), last_child->definition);
            printf("\t%s: %s\n", pretty_chord(id.arr[id_position]), definition);
            printf("\t%i, %i\n", last_child->id, id.arr[id_position]);
            exit(1);
        } else {
            last_child->definition = definition;
        }
        return dict;
    } else {
        dict.children[dict.size - 1] = push_entry(*last_child, id, id_position + 1, definition);
        return dict;
    }
}

void print_entries(dictionary dict, int depth) {
    for (int i = 0; i < depth; i++) {
        printf("\t");
    }
    printf("%i: %s\n", dict.id, dict.definition);
    int i = 0;
    for (; i < dict.size; i++) {
        print_entries(dict.children[i], depth + 1);
    }
    for (int i = 0; i < depth; i++) {
        printf("\t");
    }
    printf(" > total entries: %i\n", i);
}

json_entry parse_line(char *line) {
    int i = 0;
    json_entry entry;
    entry.value = (char*)calloc(MAX_LINE_LEN, sizeof(char));
    char c;
    while (c = line[i]) {
        switch (c) {
            case '"':
                i++;
                break;
            case ' ':
            case '\t':
                i++;
                continue;
            default:
                printf("Parsing error\n");
                entry.value = NULL;
                return entry;
        }
        break;
    }

    if (line[i] == '\0') {
        printf("Parsing error\n");
        entry.value = NULL;
        return entry;
    }

    entry.key = parse_pretty_chord(line, &i, '"');
    i++;

    while (c = line[i]) {
        switch (c) {
            case '"':
                i++;
                break;
            case ' ':
            case '\t':
            case ':':
                i++;
                continue;
            default:
                printf("parsing error\n");
                entry.value = NULL;
                return entry;
        }
        break;
    }

    if (line[i] == '\0') {
        printf("Parsing error\n");
        entry.value = NULL;
        return entry;
    }

    int is_escaped = 0;
    int value_position = 0;
    while (c = line[i++]) {
        if (is_escaped) {
            is_escaped = 0;
            switch (c) {
                case 't':
                    entry.value[value_position++] = '\t';
                    break;
                default:
                    entry.value[value_position++] = c;
                    break;
            }
            continue;
        } else {
            switch (c) {
                case '"':
                    break;
                case '\\':
                    is_escaped = 1;
                    continue;
                default:
                    entry.value[value_position++] = c;
                    continue;
            }
            break;
        }
    }

    entry.value = (char*)realloc(entry.value, ++value_position * sizeof(char));
    return entry;
}

dictionary parse_dictionary (const char* file_path) {
    FILE *file_pointer = fopen(file_path, "r");
    char line_buffer[MAX_LINE_LEN];
    
    if (file_pointer == NULL) {
        printf("Error opening file\n");
        exit(1);
    }

    dictionary dict = init_dict(0, NULL, NULL);
    json_entry entry;
    
    for (int i = 0; fgets(line_buffer, MAX_LINE_LEN, file_pointer) != NULL; i++) {
        switch (line_buffer[0]) {
            case '{':
            case '}':
                continue;
            default:
                entry = parse_line(line_buffer);
                dict = push_entry(dict, entry.key, 0, entry.value);
        }
        
    }

    fclose(file_pointer);

    return dict;
}


 
int main(int argc, char **argv) {
    dictionary* dict_list = (dictionary*)malloc((argc - 1) * sizeof(dictionary));

    for (int i = 1; i < argc; i++) {
        dict_list[i-1] = parse_dictionary(argv[i]);
    }
    
    return 0;
}
