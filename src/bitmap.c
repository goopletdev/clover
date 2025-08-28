#include "bitmap.h"

#define MAX_SHIFT (int)(CHAR_BIT * sizeof(unsigned long))

const int clover__ascii_hex_table[128] = {
    [0 ... 127] = -1,
    ['0'] = 0,
    ['1'] = 1,
    ['2'] = 2,
    ['3'] = 3,
    ['4'] = 4,
    ['5'] = 5,
    ['6'] = 6,
    ['7'] = 7,
    ['8'] = 8,
    ['9'] = 9,
    ['a'] = 10, ['A'] = 10,
    ['b'] = 11, ['B'] = 11,
    ['c'] = 12, ['C'] = 12,
    ['d'] = 13, ['D'] = 13,
    ['e'] = 14, ['E'] = 14,
    ['f'] = 15, ['F'] = 15,
};

int clover_free_bitmap(clover_bitmap m) {
    if (m.words) {
        free(m.words);
        return 0;
    }
    printf("bitmap m at m.words is NULL:");
    return 1;
}

clover_bitmap clover_parse_hexmap(const char* map) {
    clover_bitmap hexmap = { NULL, 0 };
    unsigned long value = 0;
    char c;
    int hex_value;
    while ((c = *(map++)) != '\0') {
        if ((hex_value = clover__ascii_hex_table[(int)c]) < 0) {
            hexmap = clover_parse_hexmap(map);
            hexmap.len++;
            hexmap.words = (unsigned long*)
                realloc(hexmap.words, sizeof(long)*hexmap.len);
            hexmap.words[hexmap.len - 1] = value;
            break;
        }
        value *= 16;
        value += hex_value;
    }
    return hexmap;
}

clover_bitmap clover_init_empty_bitmap(int len) {
    clover_bitmap m;
    m.len = len;
    m.words = len ? (unsigned long*)calloc(len, sizeof(unsigned long)) : NULL;
    return m;
}

clover_bitmap clover_bitmap_add_value(clover_bitmap m, int val) {
    int bitmap_position = 0;
    while (val >= MAX_SHIFT) {
        val -= MAX_SHIFT;
        if (++bitmap_position >= m.len) {
            m.len = bitmap_position + 1;
            m.words = (unsigned long*)realloc(m.words, sizeof(long) * m.len);
        }
    }
    if (!m.words) {
        m.len = 1;
        m.words = (unsigned long*)calloc(m.len, sizeof(unsigned long));
    }
    unsigned long bitmask = 1UL << val;
    m.words[bitmap_position] |= bitmask;
    return m;
}

clover_bitmap clover_bitmap_from_array(const int* vals, int len) {
    clover_bitmap m = { NULL, 0 };
    for (int i = 0; i < len; i++) {
        m = clover_bitmap_add_value(m, vals[i]);
    }
    return m;
}

int clover_bitmap_bit_is_high(clover_bitmap m, int bit_position) {
    int bitmap_position = 0;
    while (bit_position >= MAX_SHIFT) {
        bit_position -= MAX_SHIFT;
        bitmap_position++;
    }
    if (bitmap_position >= m.len) {
        return 0;
    }
    unsigned long bitmask = 1UL << bit_position;
    if (m.words[bitmap_position] & bitmask) {
        return 1;
    } else {
        return 0;
    }
}

clover_bitmap clover_bitmap_set_bit_high(clover_bitmap m, int bit_pos) {
    int bitmap_position = 0;
    while (bit_pos >= MAX_SHIFT) {
        bit_pos -= MAX_SHIFT;
        bitmap_position++;
    }
    if (bitmap_position >= m.len) {
        printf("bit position out of range of bitmap\n");
        return m;
    }
    unsigned long bitmask = 1UL << bit_pos;
    m.words[bitmap_position] |= bitmask;
    return m;
}

clover_bitmap clover_bitmap_clear_bit(clover_bitmap m, int bit_pos) {
    int bitmap_position = 0;
    while (bit_pos >= MAX_SHIFT) {
        bit_pos -= MAX_SHIFT;
        bitmap_position++;
    }
    if (bitmap_position >= m.len) {
        printf("bit position out of range of bitmap\n");
        return m;
    }
    unsigned long bitmask = 1UL << bit_pos;
    m.words[bitmap_position] &= ~bitmask;
    return m;
}

