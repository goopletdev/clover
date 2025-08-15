#include "./clvrlib.h"
#include <stdio.h>
#include <stdlib.h>

int configure(void) {
    printf("Opening config.");
    FILE *fp = fopen("./config.toml", "r");
    putchar('.');
    if (fp == NULL) {
        printf("Error opening config\n");
        exit(1);
    }
    char line_buffer[MAX_LINE_LEN];
    putchar('.\n');

    for (int i = 0; fgets(line_buffer, MAX_LINE_LEN, fp) != NULL; i++) {

    }
}

int main(int argc, char **argv) {

    return 0;
}