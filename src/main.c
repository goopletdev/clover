#include "bitmap.h"
#include "chord.h"
#include "event-listener.h"
#include "event-emulator.h"
#include "dict.h"
#include "json.h"

#include <fcntl.h>
#include <limits.h> // CHAR_BIT
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libevdev/libevdev-uinput.h>

#include "tomlc17.h"

#define COL_RESET "\x1B[0m"
#define COL_RED "\x1B[31m"
#define COL_GREEN "\x1B[32m"
#define COL_YELLOW "\x1B[33m"
#define COL_BLUE "\x1B[34m"
#define COL_MAGENTA "\x1B[35m"
#define COL_CYAN "\x1B[36m"
#define COL_WHITE "\x1B[37m"

const int BUFFER_SIZE = 1024;

// temporary escape check until commands are implemented
char* ESCAPE_CHORD = "PHROLG";
clover_chord ESCAPE;

// variable declaration
clover_dict* d;
clover_history* hist;
int kbd_fd, uinput_fd, grab = 1, arpeggiate = 1;
struct libevdev* kbd_dev;
struct libevdev_uinput* virtkbd_dev;

void clover__cleanup(void) {
    printf("cleaning up...\n");
    close(kbd_fd);
    close(uinput_fd);
    clover_free_dict(d);
    clover_history_free(hist);
    printf("DONE\n");
}

void clover__parse_config(void) {
    /**********************************
     * OPEN CONFIG
     *********************************/
    toml_result_t result = toml_parse_file_ex("config.toml");
    if (!result.ok) {
        printf("config.toml missing or broken. Trying default-config.toml\n");
        result = toml_parse_file_ex("default-config.toml");
    }
    if (!result.ok) {
        printf("Could not parse .toml\n");
        exit(1);
    }
    /**********************************
     * STENO DICTIONARY PARSING
     *********************************/
    // get dictionaries path
    toml_datum_t dict_path = toml_seek(result.toptab, "dictionary.path");
    if (dict_path.type != TOML_STRING) {
        printf("missing or broken 'dictionary.path' property in config\n");
        exit(1);
    }
    // get array of dict paths relative to dict directory
    toml_datum_t dict_array = toml_seek(result.toptab, "dictionary.dictionaries");
    if (dict_array.type != TOML_ARRAY) {
        printf("missing or broken dictionary.dictionaries prop in config\n");
        exit(1);
    }
    // parse each dictionary into single dict object
    d = clover_table_init_dict(0, NULL);
    char buffer[BUFFER_SIZE];
    for (int i = 0; i < dict_array.u.arr.size; i++) {
        toml_datum_t elem = dict_array.u.arr.elem[i];
        if (elem.type != TOML_STRING) {
            printf("dictionary.dictionaries element is not a string\n");
            exit(1);
        }
        memset(buffer, 0, BUFFER_SIZE);
        strcpy(buffer, dict_path.u.str.ptr);
        strcat(buffer, elem.u.str.ptr);
        d = clover_parse_dictionary(buffer, d);
    }
    printf("Done parsing steno dicts. Root dict size: %i\n", d->children.size);
    /**********************************
     * KEYBOARD EVENTS
     *********************************/
    toml_datum_t event_path_toml = toml_seek(result.toptab, "keyboard.path");
    if (event_path_toml.type != TOML_STRING) {
        printf("missing or invalid 'keyboard.path' property in config\n");
        exit(1);
    }
    if ((kbd_fd = open(event_path_toml.u.str.ptr, O_RDONLY)) == -1) {
        printf("Error opening event %s\n", event_path_toml.u.str.ptr);
        exit(1);
    }
    if ((uinput_fd = open("/dev/uinput", O_RDWR)) == -1) {
        printf("Error opening /dev/uinput\n");
    }
    libevdev_new_from_fd(kbd_fd, &kbd_dev);
    libevdev_uinput_create_from_device(kbd_dev, uinput_fd, &virtkbd_dev);
    ioctl(kbd_fd, EVIOCGRAB, &grab);
    /**********************************
     * OTHER STUFF
     *********************************/
    toml_datum_t arpeggiate_toml = toml_seek(result.toptab, "machine.arpeggiate");
    if (arpeggiate_toml.type != TOML_BOOLEAN) {
        if (arpeggiate_toml.type == TOML_UNKNOWN) {
            printf("arpeggiate value not found. setting to default\n");
            arpeggiate = 1;
        } else {
            printf("unsupported arpeggiate type in config\n");
            exit(1);
        }
    } else if (!arpeggiate_toml.u.boolean) {
        printf("Currently, only arpeggiate mode is supported. Ignoring config value\n");
        arpeggiate = 1;
    }
    /**********************************
     * CLEANUP
     *********************************/
    toml_free(result);
    return;
}

void clover__handle_cli_args(int argc, char **argv) {
    if (argc == 1) {
        printf("Running %s with no args...\n", argv[0]);
    } else {
        printf("Running %s...\n", argv[0]);
        for (int i = 0; i < argc; i++) {
            printf("Arg %i: %s\n", i, argv[i]);
        }
    }
    // ESCAPE sequence until commands are implemented
    ESCAPE = clover_parse_chord(ESCAPE_CHORD);
    printf("To close Clover: interrupt or chord \"%s\" on captive device.\n", ESCAPE_CHORD);
}

void clover__send_chord(struct libevdev_uinput* uinput_dev, clover_chord chord, clover_dict* dict) {
    putchar('\r');
    clover_put_tape(chord);
    printf(" | ");
    clover_dict* entry = clover_dict_get(dict, chord);
    char* translation;
    if (entry && entry->translations.entries) {
        translation = (char*)malloc((strlen(entry->translations.entries[0]) + 1) * sizeof(char));
        strcpy(translation, entry->translations.entries[0]);
        printf("%s", translation);
        if (entry->children.size) {
            printf("%s -> ... (%i)%s\n", COL_MAGENTA, entry->children.size, COL_RESET);
        } else {
            putchar('\n');
        }
    } else {
        translation = clover_pretty_chord(chord);
        printf("%s%s%s", COL_YELLOW, translation, COL_RESET);
        if (entry && entry->children.size) {
            printf("%s -> ... (%i)%s\n", COL_MAGENTA, entry->children.size, COL_RESET);
        } else {
            putchar('\n');
        }
    }

    if (chord == ESCAPE) {
        // temporary escape check until commands are implemented
        printf("\rEscape sequence {PLOVER:TOGGLE}\n");
        free(translation);
        exit(0);
    } else {
        send_string(uinput_dev, translation);
        send_string(uinput_dev, " ");
        free(translation);
    }
}

void clover__event_loop(int fd, struct libevdev_uinput* uinput_dev, clover_dict* dict) {   
    clover_chord chord = 0;
    int keys_down = 0;

    printf("%sREADY%s\n", COL_GREEN, COL_RESET);

    for (;;) {
        chord = clover_handle_key_event(fd, chord, &keys_down);
        if (clover_chord_is_ready(chord)) {
            clover__send_chord(uinput_dev, clover_chord_value(chord), dict);
            chord = 0;
        }
    }
}

int main(int argc, char** argv) {
    /**********************************
     * EXIT FUNCTION
     *********************************/
    atexit(clover__cleanup);

    /**********************************
     * ARGS
     *********************************/
    // placeholder to avoid compiler warnings until later implementation
    clover__handle_cli_args(argc, argv);

    /**********************************
     * CONFIG PARSING
     *********************************/
    clover__parse_config();

    /**********************************
     * INITIALIZE HISTORY AND STUFF
     *********************************/
    hist = clover_history_init();
    clover_instruction_start();

    /**********************************
     * MAIN LOOP
     *********************************/
    clover__event_loop(kbd_fd, virtkbd_dev, d);

    return 0;
}
