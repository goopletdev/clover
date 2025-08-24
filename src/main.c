#include "clover-chord/clover-chord.h"
#include "clover-event-listener/clover-event-listener.h"
#include "clover-event-emulator/clover-event-emulator.h"
#include "clover-hash/clover-hash.h"
#include "clover-json/clover-json.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libevdev/libevdev-uinput.h>

#define EVENT_PATH "/dev/input/event23"
#define DICTIONARY_PATH "./assets/lapwing-base.json"

void handle_cli_args(int argc, char **argv) {
    printf("Running ");
    for (int i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);
    }
}

void clover__send_chord(struct libevdev_uinput* uinput_dev, clover_chord chord, clover_dict* dict) {
    printf("\r%s | ", clover_paper_tape(chord));
    clover_dict* entry = clover_get(dict, chord);
    const char* translation = entry ? clover_dict_translation(entry) : NULL;
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

int main(int argc, char **argv) {
    handle_cli_args(argc, argv);

    int kbd_fd = open(EVENT_PATH, O_RDONLY);
    if (kbd_fd == -1) {
        printf("Error opening event handler\n");
        exit(1);
    }
    struct libevdev *kbd_dev;
    libevdev_new_from_fd(kbd_fd, &kbd_dev);

    struct libevdev_uinput* virtkbd_dev;
    int uinput_fd = open("/dev/uinput", O_RDWR);
    libevdev_uinput_create_from_device(kbd_dev, uinput_fd, &virtkbd_dev);
    const char *virtkbd_path = libevdev_uinput_get_devnode(virtkbd_dev);
    printf("Virtual keyboard device: %s\n", virtkbd_path);


    printf("Parsing dictionary...\n");
    clover_dict* dict = clover_parse_dictionary(DICTIONARY_PATH);
    if (dict == NULL) {
        printf("Error parsing dictionary\n");
        exit(1);
    }
    printf("Finished parsing dictionary. Size: %i\n", clover_dict_size(dict));

    int grab = 1;
    ioctl(kbd_fd, EVIOCGRAB, &grab);

    clover__event_loop(kbd_fd, virtkbd_dev, dict);

    close(kbd_fd);
    return 0;
}
