// simulates keyboard output

#include "./clvrlib.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <libevdev/libevdev-uinput.h>

int main(int argc, char **argv) {
    struct input_event event;

    int kbd_fd;
    int uinput_fd;
}