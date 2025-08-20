#include "./clover-json.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

clover_dict* clover_parse_dictionary(const char* file_path) {
    printf("Opening JSON from %s...\n", file_path);
    FILE* fp = fopen(file_path, "r");
    if (fp == NULL) {
        printf("Error opening dictionary %s\n", file_path);
        exit(1);
    }

    clover_dict* d = clover_init_dict(0, NULL, NULL);
    if (d == NULL) {
        printf("Failed dictionary initialization\n");
        exit(1);
    }
    char key_buffer[CHAR_BUFFER_SIZE] = {0};
    int key_position = 0;
    char val_buffer[CHAR_BUFFER_SIZE] = {0};
    int val_position = 0;

    // 0: before key; 1: in key; 2: after key; 3: in val; 4: after val
    int keyval_pos = 0;

    int is_escaped = 0;
    int c;
    while ((c = fgetc(fp)) != EOF) {
        if (is_escaped) {
            is_escaped = 0;
            if (keyval_pos == 0 || keyval_pos == 2) {
                continue;
            }
            switch ((char)c) {
                case 'n':
                    c = '\n';
                    break;
                case 't':
                    c = '\t';
                    break;
                case 'b':
                    c = '\b';
                    break;
                case 'r':
                    c = '\r';
                    break;
            }
            if (keyval_pos == 1) {
                key_buffer[key_position++] = (char)c;
            } else if (keyval_pos == 3) {
                val_buffer[val_position++] = (char)c;
            }
        } else if ((char)c == '"') {
            if (++keyval_pos == 4) {
                keyval_pos = 0;
                key_position = 0;
                val_position = 0;
                int size = 0;
                unsigned int* id;
                id = clover_parse_compound_chord(key_buffer, &size);
                d = clover_push_entry(d, id, size, val_buffer);
                memset(key_buffer, 0, CHAR_BUFFER_SIZE * sizeof(char));
                memset(val_buffer, 0, CHAR_BUFFER_SIZE * sizeof(char));
            }
        } else if ((char)c == '\\') {
            is_escaped = 1;
        } else if (keyval_pos == 1) {
            key_buffer[key_position++] = (char)c;
        } else if (keyval_pos == 3) {
            val_buffer[val_position++] = (char)c;
        } else {
        }
    }

    printf("finished parsing; closing file...\n");

    fclose(fp);
    printf("file closed.\n");

    return d;
}