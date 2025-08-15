#include "./clvrlib.h"
#include <stdio.h>
#include <stdlib.h>

struct config configure(void) {
    struct config cf;

    // not yet implemented:
    printf("Opening config.");
    FILE *fp = fopen("./config.toml", "r");
    putchar('.');
    if (fp == NULL) {
        printf("Error opening config\n");
        exit(1);
    }
    char line_buffer[MAX_LINE_LEN];
    printf(".\n");

    for (int i = 0; fgets(line_buffer, MAX_LINE_LEN, fp) != NULL; i++) {
        printf("%s", line_buffer);
    }
    putchar('\n');

    // temporary configuration not using config file:
    cf.dictionary = parse_dictionary("./assets/lapwing-base.json");
    char* listeners[1] = {"/dev/input/event19"};
    cf.listeners = listeners;

    return cf;
}

int main(int argc, char **argv) {
    struct config cf = configure();
    event_loop(cf);
    return 0;
}