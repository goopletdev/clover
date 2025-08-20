#include "clover-chord/clover-chord.h"
#include "clover-event-listener/clover-event-listener.h"
#include "clover-hash/clover-hash.h"
#include "clover-json/clover-json.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void clover__send_chord(unsigned int chord, clover_dict* dict) {
    printf("\r%s | ", clover_paper_tape(chord));
    clover_dict* entry = clover_get(dict, chord);
    const char* translation = entry ? clover_dict_translation(entry) : NULL;
    printf("%s\n", translation ? translation : clover_pretty_chord(chord));
}

void clover__event_loop(int fd, clover_dict* dict) {   
    unsigned int chord = 0;
    int keys_down = 0;

    for (;;) {
        chord = clover_handle_key_event(fd, chord, &keys_down);
        if (clover_chord_is_ready(chord)) {
            clover__send_chord(clover_chord_value(chord), dict);
            chord = 0;
        }
    }
}

int main(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        printf("%s\n", argv[i]);
    }


    char event_path[] = "/dev/input/event23";
    int fd = open(event_path, O_RDONLY);
    if (fd == -1) {
        printf("Error opening event handler\n");
        exit(1);
    }

    printf("Parsing dictionary...\n");
    clover_dict* dict = clover_parse_dictionary("./assets/lapwing-base.json");
    if (dict == NULL) {
        printf("Error parsing dictionary\n");
        exit(1);
    }
    printf("Finished parsing dictionary. Size: %i\n", clover_dict_size(dict));

    clover__event_loop(fd, dict);

    close(fd);
    return 0;
}