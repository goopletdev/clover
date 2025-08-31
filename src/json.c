#include "json.h"

enum keyval_position {
    BEFORE_KEY,
    IN_KEY,
    AFTER_KEY,
    IN_VAL,
    AFTER_VAL,
};

clover_dict* clover_parse_dictionary(const char* file_path, clover_dict* d) {
    printf("Opening JSON from %s...\n", file_path);
    FILE* fp = fopen(file_path, "r");
    if (fp == NULL) {
        printf("Error opening dictionary %s\n", file_path);
        exit(1);
    }

    char key_buffer[CHAR_BUFFER_SIZE] = {0};
    int key_position = 0;
    char val_buffer[CHAR_BUFFER_SIZE] = {0};
    int val_position = 0;

    enum keyval_position kv_pos = BEFORE_KEY;

    int is_escaped = 0;
    int c;
    while ((c = fgetc(fp)) != EOF) {
        if (is_escaped) {
            is_escaped = 0;
            if (kv_pos == BEFORE_KEY || kv_pos == AFTER_KEY) {
                continue;
            }
            /*
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
            */
            if (kv_pos == IN_KEY) {
                if ((char)c != '"') {
                    key_buffer[key_position++] = '\\';
                }
                key_buffer[key_position++] = (char)c;
            } else if (kv_pos == IN_VAL) {
                if ((char)c != '"') {
                    val_buffer[val_position++] = '\\';
                }
                val_buffer[val_position++] = (char)c;
            }
        } else if ((char)c == '"') {
            if (++kv_pos == AFTER_VAL) {
                kv_pos = BEFORE_KEY;
                key_position = 0;
                val_position = 0;
                int size = 0;
                clover_chord* id;
                id = clover_parse_compound_chord(key_buffer, &size);
                d = clover_push_entry(d, id, size, val_buffer);
                memset(key_buffer, 0, CHAR_BUFFER_SIZE * sizeof(char));
                memset(val_buffer, 0, CHAR_BUFFER_SIZE * sizeof(char));
            }
        } else if ((char)c == '\\') {
            is_escaped = 1;
        } else if (kv_pos == IN_KEY) {
            key_buffer[key_position++] = (char)c;
        } else if (kv_pos == IN_VAL) {
            val_buffer[val_position++] = (char)c;
        } else {
        }
    }

    printf("finished parsing; closing file...\n");

    fclose(fp);
    printf("file closed.\n");

    return d;
}
