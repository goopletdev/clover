#include "./clvrlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct json_entry {
    uint_arr key;
    char* value;
} json_entry;

void print_entries(dict* entry, int depth) {
    for (int i = 0; i < depth; i++) {
        printf("\t");
    }
    printf("%s: %s\n", pretty_chord(entry->id), entry->translation);
    for (int j = 0; j < depth; j++) {
        printf("\t");
    }
    printf(" > total entries: %i\n", entry->children->size);
    
    for (int i = 0; i < entry->children->capacity; i++) {
        if (!entry->children->dicts[i]) {
            continue;
        }
        print_entries(entry->children->dicts[i], depth + 1);
    }
}

uint_arr parse_pretty_chord(char* chord, int* start, char terminator) {
    uint_arr id;
    id.size = 1;
    id.arr = (unsigned int*)calloc(id.size, sizeof(unsigned int));
    check_memory_allocation(id.arr, 1);

    int search_start = 0;
    for (char key; (key = chord[*start]) != terminator; (*start)++) {
        search_start = steno_index_of(key, search_start);
        if (search_start == -1) {
            switch (key) {
                case '-':
                    search_start = RIGHT_START;
                    break;
                case '/':
                    search_start = 0;
                    id.arr = (unsigned int*)realloc(id.arr, ++id.size * sizeof(unsigned int));
                    check_memory_allocation(id.arr, 1);
                    break;
                default:
                    printf("Error parsing chord \"%s\"\n", chord);
                    exit(1);
            }
        } else {
            id.arr[id.size - 1] |= 1U << search_start++;
        }
    }

    return id;
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

dict* parse_dictionary (const char* file_path) {
    FILE *file_pointer = fopen(file_path, "r");
    char line_buffer[MAX_LINE_LEN];
    
    if (file_pointer == NULL) {
        printf("Error opening file\n");
        exit(1);
    }

    dict* d = init_dict(0, NULL, NULL);
    json_entry entry;
    
    for (int i = 0; fgets(line_buffer, MAX_LINE_LEN, file_pointer) != NULL; i++) {
        switch (line_buffer[0]) {
            case '{':
            case '}':
                continue;
            default:
                entry = parse_line(line_buffer);
                d = push_entry(d, entry.key, 0, entry.value);
        }
        
    }

    fclose(file_pointer);

    return d;
}
/*
int main(int argc, char **argv) {
    dict** dict_list = (dict**)malloc((argc - 1) * sizeof(dict*));

    for (int i = 1; i < argc; i++) {
        dict_list[i-1] = parse_dictionary(argv[i]);
    }

    for (int i = 0; i < argc - 1; i++) {
        print_entries(dict_list[i], 0);
    }

    return 0;
}
*/