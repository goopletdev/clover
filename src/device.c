/*#include "device.h"

#include <dirent.h>
#include <limits.h>
#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEV_EVENT_PATH "/dev/input/%s"
#define SYS_PATH "/sys/class/input/"
#define KEY_PATH "/sys/class/input/%s/device/capabilities/key"
#define NAME_PATH "/sys/class/input/%s/device/name" 

struct clover__bitmapT {
    unsigned long* map;
    int len;
};

struct clover__deviceT {
    char* name;
    char* event_path;
    struct clover__bitmapT keys;
    struct clover__bitmapT state;
    int keys_down;
};

struct clover__device_listT {
    struct clover__deviceT* devices;
    int len;
};

int clover_free_bitmap(clover_bitmap m) {
    free(m.map);
    return 0;
}

int clover_free_device(clover_device d) {
    clover_free_bitmap(d.keys);
    clover_free_bitmap(d.state);
    free(d.name);
    free(d.event_path);
    return 0;
}

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

clover_bitmap clover_parse_hexmap(char* map) {
    clover_bitmap hexmap = { NULL, 0 };
    unsigned long value = 0;
    char c;
    int hex_value;
    while ((c = *(map++)) != '\0') {
        if ((hex_value = clover__ascii_hex_table[(int)c]) < 0) {
            hexmap = clover_parse_hexmap(map);
            hexmap.len++;
            hexmap.map = (unsigned long*)
                realloc(hexmap.map,sizeof(long)*hexmap.len);
            hexmap.map[hexmap.len - 1] = value;
            break;
        }
        value *= 16;
        value += hex_value;
    }
    return hexmap;
}

clover_bitmap init_empty_bitmap(int len) {
    clover_bitmap m;
    m.len = len;
    m.map = (unsigned long*)calloc(len, sizeof(unsigned long));
    return m;
}

clover_bitmap clover_bitmap_add_value(clover_bitmap m, int val) {
    int max_shift = CHAR_BIT * sizeof(unsigned long);
    int bitmap_position = 0;
    while (value >= max_shift) {
        value -= max_shift;
        if (++bitmap_position >= m.len) {
            m.len = bitmap_position + 1;
            m.map = (unsigned long*)realloc(m.map, sizeof(long) * m.len);
        }
    }
    unsigned long bitmask = 1UL << value;
    m.map[bitmap_position] &= bitmask;
    return m;
}

clover_bitmap clover_bitmap_from_array(const int* vals, int len) {
    int max_shift = CHAR_BIT * sizeof(unsigned long);
    int bitmap_position = -1;
    clover_bitmap m = { NULL, 0 };
    for (int i = 0; i < len; i++) {
        m = clover_bitmap_add_value(vals[i]);
    }
    return m;
}


char* get_text_from_file(char* format_str, char* dirname) {
    char buffer[1000] = {'\0'};

    snprintf(buffer, sizeof(buffer), format_str, dirname);
    FILE* fp = fopen(buffer, "r");
    if (fp == NULL) {
        printf("Could not open file %s\n", buffer);
        return NULL;
    }

    memset(buffer, 0, sizeof(buffer));

    char c;
    int count = 0;
    while ((c = fgetc(fp)) != EOF) {
        buffer[count++] = c;
        if (count >= 1000) {
            printf("Char count exceeds buffer size.");
            exit(1);
        }
    }

    char* text = (char*)calloc(strlen(buffer) + 1, sizeof(char));
    fclose(fp);
    return strcpy(text, buffer);
}

char* slice_to_newline(char* str) {
    for (int i = 0; str[i]; i++) {
        if (str[i] == '\n') {
            str[i] = '\0';
            str = (char*)realloc(str, (strlen(str) + 1) * sizeof(char));
            break;
        }
    }
    return str;
}

clover_device clover_get_device(char* event_name) {
    char buffer[100] = {'\0'};
    clover_device device;
    snprintf(buffer, sizeof(buffer), DEV_EVENT_PATH, event_name);
    device.event_path = (char*)malloc(sizeof(char)*(strlen(buffer) + 1));
    strcpy(device.event_path, buffer);

    device.name = slice_to_newline(get_text_from_file(NAME_PATH, event_name));
    char* keys = get_text_from_file(KEY_PATH, event_name);
    device.keys = clover_parse_hexmap(keys);
    free(keys);
    device.state = init_empty_bitmap(device.keys.len);
    return device;
}

clover_device_list get_devices(void) {
    clover_device_list dl = { NULL, 0 };
    struct dirent* de;
    DIR dr = opendir(SYS_PATH);

    if (dr == NULL) {
        printf("Could not open %s\n", SYS_PATH);
        exit(1);
    }

    while ((de = readdir(dr)) != NULL) {
        if (de->d_name[0] != 'e') continue;
        clover_device device = clover_get_device(de->d_name);
        dl.len++;
        dl.devices = (clover_device*)realloc(dl.devices, dl.len * sizeof(clover_device));
        dl.devices[dl.len - 1] = device;
    }
    closedir(dr);
    return dl;
}

clover_device_list clover_prune_devices(int (*callback)(clover_device), clover_device_list devices) {
    clover_device_list dl = { NULL, 0 };
    for (int i = 0; i < devices.len; i++) {
        if (callback(devices.devices[i])) {
            dl.devices = (clover_device*)realloc(dl.devices, ++dl.len * sizeof(clover_device));
        } else {
            clover_free_device(&devices.devices[i]);
        }
    }
    free(devices.devices);
    return dl;
}

int clover_bitmap_has_value(clover_bitmap m, int value) {
    int max_shift = CHAR_BIT * sizeof(unsigned long);
    int bitmap_position = 0;
    while (value >= max_shift) {
        value -= max_shift;
        bitmap_position++;
    }
    if (bitmap_position >= m.len) {
        return 0;
    }
    unsigned long bitmask = 1UL << value;
    if (m.map[bitmap_position] & bitmask) {
        return 1;
    } else {
        return 0;
    }
}

int clover_device_has_key(clover_device d, int key) {
    return clover_bitmap_has_value(d.keys, key);
} 
*/
