#include "json.h"

#define CHORD_BUFFER_SIZE 512

enum clover__json_keyval_position {
    BEFORE_KEY,
    IN_KEY,
    AFTER_KEY,
    IN_VAL,
    AFTER_VAL,
};

clover_dict* clover_json_parse(const char* file_path, clover_dict* d) {
    FILE* fp = fopen(file_path, "r");
    if (fp == NULL) {
        printf("Error opening dictionary %s\n", file_path);
        exit(1);
    }

    char key_buffer[CHAR_BUFFER_SIZE] = {0};
    int key_position = 0;
    char val_buffer[CHAR_BUFFER_SIZE] = {0};
    int val_position = 0;
    clover_chord chord_buffer[CHORD_BUFFER_SIZE] = {0};

    enum clover__json_keyval_position kv_pos = BEFORE_KEY;

    int is_escaped = 0;
    int c;
    while ((c = fgetc(fp)) != EOF) {
        if (is_escaped) {
            is_escaped = 0;
            if (kv_pos == BEFORE_KEY || kv_pos == AFTER_KEY) {
                continue;
            }
            if (kv_pos == IN_KEY) {
                if ((char)c != '"') {
                    key_buffer[key_position++] = '\\';
                }
                val_buffer[val_position++] = (char)c;
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
                clover_parse_compound_chord(chord_buffer, CHORD_BUFFER_SIZE, key_buffer);
                clover_dict_add_entry(d, chord_buffer, val_buffer);
                memset(key_buffer, 0, CHAR_BUFFER_SIZE);
                memset(val_buffer, 0, CHAR_BUFFER_SIZE);
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

    fclose(fp);
    return d;
}


