#include "chord.h"
#include "event-listener.h"
#include "event-emulator.h"
#include "table.h"
#include "json.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libevdev/libevdev-uinput.h>

#include "tomlc17.h"

const int BUFFER_SIZE = 1024;

void handle_cli_args(int argc, char **argv) {
    printf("Running ");
    for (int i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);
    }
}

void clover__send_chord(struct libevdev_uinput* uinput_dev, clover_chord chord, clover_dict* dict) {
    printf("\r%s | ", clover_paper_tape(chord));
    clover_dict* entry = clover_dict_get(dict, chord);
    const char* translation = entry ? entry->translations.entries[0] : NULL;
    if (!translation) {
        translation = clover_pretty_chord(chord);
    }
    printf("%s\n", translation);
    send_string(uinput_dev, translation);
    send_string(uinput_dev, " ");
}

void clover__event_loop(int fd, struct libevdev_uinput* uinput_dev, clover_dict* dict) {   
    clover_chord chord = 0;
    int keys_down = 0;

    for (;;) {
        chord = clover_handle_key_event(fd, chord, &keys_down);
        if (clover_chord_is_ready(chord)) {
            clover__send_chord(uinput_dev, clover_chord_value(chord), dict);
            chord = 0;
        }
    }
}

int main(int argc, char** argv) {
    /******************************************************
     * ARGS
     *****************************************************/
    // placeholder to avoid compiler warnings until later implementation
    handle_cli_args(argc, argv);

    /******************************************************
     * CONFIG PARSING
     *****************************************************/
    toml_result_t result = toml_parse_file_ex("config.toml");
    if (!result.ok) {
        printf("Could not parse .toml\n");
        exit(1);
    }
    printf("Dictionary path: %s\n", dict_path.u.str.ptr);

    /******************************************************
     * DICTIONARY PARSING
     *****************************************************/
    // get dictionary path
    toml_datum_t dict_path = toml_seek(result.toptab, "dictionary.path");
    if (dict_path.type != TOML_STRING) {
        printf("missing or invalid 'dictionary.path' property in config\n");
        exit(1);
    }

    // get array of dictionary paths
    toml_datum_t dict_array = toml_seek(result.toptab, "dictionary.dictionaries");
    if (dict_array.type != TOML_ARRAY) {
        printf("missing or invalid 'dictionary.dictionaries' property in config");
        exit(1);
    }

    // initialize clover dictionary
    clover_dict* d = clover_table_init_dict(0, NULL);

    // parse each dictionary into single dictionary object
    for (int i = 0; i < dict_array.u.arr.size; i++) {
        toml_datum_t elem = dict_array.u.arr.elem[i];
        if (elem.type != TOML_STRING) {
            printf("dictionary.dictionaries element is not a string");
            exit(1);
        }
        printf("dictionary [%i]: %s\n", i, elem.u.str.ptr);
        char buffer[BUFFER_SIZE] = {'\0'};
        strcpy(buffer, dict_path);
        strcat(buffer, file_name);
        d = clover_parse_dictionary(buffer, d);
    }
    printf("Finished parsing dictionary. Size: %i\n", d->children.size);

    /******************************************************
     * KEYBOARD EVENT
     *****************************************************/
    // get event path
    toml_datum_t event_path_toml = toml_seek(result.toptab, "keyboard.path");
    if (event_path.type != TOML_STRING) {
        printf("missing or invalid 'keyboard.path' property in config\n");
        exit(1);
    }
    // get file descriptor
    int kbd_fd = open(event_path_toml.u.str.ptr, O_RDONLY);
    if (kbd_fd == -1) {
        printf("Error opening event %s\n", event_path_toml.u.str.ptr);
        exit(1);
    }

    struct libevdev *kbd_dev;
    libevdev_new_from_fd(kbd_fd, &kbd_dev);

    struct libevdev_uinput* virtkbd_dev;
    int uinput_fd = open("/dev/uinput", O_RDWR);
    libevdev_uinput_create_from_device(kbd_dev, uinput_fd, &virtkbd_dev);
    const char *virtkbd_path = libevdev_uinput_get_devnode(virtkbd_dev);
    printf("Virtual keyboard device %s\n", virtkbd_path);

    int grab = 1;
    ioctl(kbd_fd, EVIOCGRAB, &grab);



    /******************************************************
     * TOML CLEANUP
     *****************************************************/
    // free toml object
    toml_free(result);

    /******************************************************
     * MAIN LOOP
     *****************************************************/
    clover__event_loop(kbd_fd, virtkbd_dev, d);

    /******************************************************
     * CLEANUP
     *****************************************************/
    close(kbd_fd);
    close(uinput_fd);

    return 0;
}

