#include "device.h"
#include <linux/input.h> // for magic number values; delete later

#define LARGE_BUFFER_SIZE 1000
#define SMALL_BUFFER_SIZE 100

#define DEV_EVENT_PATH "/dev/input/%s"
#define SYS_PATH "/sys/class/input/"
#define KEY_PATH "/sys/class/input/%s/device/capabilities/key"
#define NAME_PATH "/sys/class/input/%s/device/name" 

// hard-coded magic values; delete later
const int REQUIRED_KEYS[] = {
    KEY_Q, KEY_A, KEY_W, KEY_S, KEY_E, KEY_D, KEY_R, KEY_F,
    KEY_C, KEY_V, KEY_T, KEY_G, KEY_Y, KEY_H, KEY_N, KEY_M,
    KEY_U, KEY_J, KEY_I, KEY_K, KEY_O, KEY_L, KEY_P,
    KEY_SEMICOLON, KEY_LEFTBRACE, KEY_APOSTROPHE,
};

int clover_free_device(clover_device d) {
    clover_free_bitmap(d.keys);
    clover_free_bitmap(d.state);
    free(d.name);
    free(d.event_path);
    return 0;
}

char* get_text_from_file(char* format_str, char* dirname) {
    char buffer[LARGE_BUFFER_SIZE] = {'\0'};

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
        if (count >= LARGE_BUFFER_SIZE) {
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
    clover_device d;
    snprintf(buffer, sizeof(buffer), DEV_EVENT_PATH, event_name);
    d.event_path = (char*)malloc(sizeof(char)*(strlen(buffer) + 1));
    strcpy(d.event_path, buffer);

    d.name = slice_to_newline(get_text_from_file(NAME_PATH, event_name));
    char* keys = get_text_from_file(KEY_PATH, event_name);
    d.keys = clover_parse_hexmap(keys);
    free(keys);
    d.state = clover_init_empty_bitmap(d.keys.len);
    return d;
}

clover_device_list clover_get_all_devices(void) {
    clover_device_list dl = { NULL, 0 };
    struct dirent* de;
    DIR* dr = opendir(SYS_PATH);

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

clover_device_list clover_prune_devices(int (*callback)(clover_device), clover_device_list devs) {
    clover_device_list dl = { NULL, 0 };
    for (int i = 0; i < devs.len; i++) {
        if (callback(devs.devices[i])) {
            dl.devices = (clover_device*)realloc(dl.devices, ++dl.len * sizeof(clover_device));
        } else {
            clover_free_device(devs.devices[i]);
        }
    }
    free(devs.devices);
    return dl;
}

int clover_device_has_key(clover_device d, int key) {
    return clover_bitmap_has_value(d.keys, key);
} 

int clover_device_has_all_keys(clover_device d, const int* keys, int keylen) {
    for (int i = 0; i < keylen; i++) {
        if (!clover_device_has_key(d, keys[i])) {
            return 0;
        }
    }
    return 1;
}

int clover_device_is_keyboard(clover_device d) {
    return clover_device_has_all_keys(d, REQUIRED_KEYS, 26);
}

clover_device clover_query_user_for_device(clover_device_list dl) {
    if (dl.len < 1) {
        printf("No recognized input device.\n");
        exit(1);
    }
    for (int i = 0; i < dl.len; i++) {
        printf("(%i) %s\n", i+1, dl.devices.name);
    }
    printf("Select keyboard (1-%i): ", dl.len);
    int device_num;
    scanf("%d", &device_num);
    if (device_num > dl.len || device_num < 1) {
        printf("Disallowed device number.\n");
        return clover_query_user_for_device(dl);
    }
    return dl.devices[device_num - 1];
}

