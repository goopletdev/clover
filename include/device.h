#ifndef CLOVER_DEVICE_H
#define CLOVER_DEVICE_H

#include "bitmap.h"

#include <dirent.h> // DIR*, struct dirent, opendir, readdir, closedir
#include <linux/input.h>
#include <stdio.h> // File, fopen, snprintf, fgetc, fclose
#include <stdlib.h> // free, exit, calloc, realloc, malloc
#include <string.h> // strlen, memset, strcpy

struct clover__deviceT {
    char* name;
    char* event_path;
    clover_bitmap keys;
    clover_bitmap state;
    int keys_down;
};

struct clover__device_listT {
    struct clover__deviceT* devices;
    int len;
};

typedef struct clover__deviceT clover_device;
typedef struct clover__device_listT clover_device_list;

/**
 * Frees malloc'd bitmap keys, bitmap state, name, event_path
 */
int clover_free_device(clover_device d);

/**
 * Dynamically generates file path, opens file, gets text, closes file
 */
char* get_text_from_file(char* format_str, char* dirname);

/**
 * Returns contents of string up to the first newline, 
 * replaces newline with null terminator, returns truncated string
 */
char* slice_to_newline(char* str);

/**
 * Creates clover_device from device associated with given event
 */
clover_device clover_get_device(char* event_name);

/**
 * Loads all devices served by an input event handler
 */
clover_device_list clover_get_all_devices(void);

/**
 * Return list of devices that match criteria 
 */
clover_device_list clover_prune_devices(
    int (*callback)(clover_device), clover_device_list devs
);

#endif // CLOVER_DEVICE_H
