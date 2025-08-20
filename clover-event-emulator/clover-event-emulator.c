#include "./clover-event-emulator.h"

#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <libevdev/libevdev-uinput.h>

struct key_combination {
    int keycode;
    unsigned int modifiers;
};

const unsigned int MOD_LEFT_SHIFT = 1U << 0;
const unsigned int MOD_RIGHT_SHIFT = 1U << 1;
const unsigned int MOD_LEFT_ALT = 1U << 2;
const unsigned int MOD_RIGHT_ALT = 1U << 3;
const unsigned int MOD_LEFT_CTRL = 1U << 4;
const unsigned int MOD_RIGHT_CTRL = 1U << 5;
const unsigned int MOD_LEFT_META = 1U << 6;
const unsigned int MOD_RIGHT_META = 1U << 7;

const unsigned int MOD_KEYS[8] = {
    [0] = KEY_LEFTSHIFT,
    [1] = KEY_RIGHTSHIFT,
    [2] = KEY_LEFTALT,
    [3] = KEY_RIGHTALT,
    [4] = KEY_LEFTCTRL,
    [5] = KEY_RIGHTCTRL,
    [6] = KEY_LEFTMETA,
    [7] = KEY_RIGHTMETA,
};

static const struct key_combination char_map[128] = {
    [0 ... 127] = { '\0', 0 },

    #define KEYCOMBO(x,y,z) [x] = { y, z }

    KEYCOMBO('\b', KEY_BACKSPACE, 0),
    KEYCOMBO('\t', KEY_TAB, 0),
    KEYCOMBO('\n', KEY_ENTER, 0),
    KEYCOMBO(' ', KEY_SPACE, 0),
    KEYCOMBO('!', KEY_1, MOD_LEFT_SHIFT),
    KEYCOMBO('"', KEY_APOSTROPHE, MOD_LEFT_SHIFT),
    KEYCOMBO('#', KEY_3, MOD_LEFT_SHIFT),
    KEYCOMBO('$', KEY_4, MOD_LEFT_SHIFT),
    KEYCOMBO('%', KEY_5, MOD_LEFT_SHIFT),
    KEYCOMBO('&', KEY_7, MOD_LEFT_SHIFT),
    KEYCOMBO('\'', KEY_APOSTROPHE, 0),
    KEYCOMBO('(', KEY_9, MOD_LEFT_SHIFT),
    KEYCOMBO(')', KEY_0, MOD_LEFT_SHIFT),
    KEYCOMBO('*', KEY_8, MOD_LEFT_SHIFT),
    KEYCOMBO('+', KEY_EQUAL, MOD_LEFT_SHIFT),
    KEYCOMBO(',', KEY_COMMA, 0),
    KEYCOMBO('-', KEY_MINUS, 0),
    KEYCOMBO('.', KEY_DOT, 0),
    KEYCOMBO('/', KEY_SLASH, 0),
    KEYCOMBO('0', KEY_0, 0),
    KEYCOMBO('1', KEY_1, 0),
    KEYCOMBO('2', KEY_2, 0),
    KEYCOMBO('3', KEY_3, 0),
    KEYCOMBO('4', KEY_4, 0),
    KEYCOMBO('5', KEY_5, 0),
    KEYCOMBO('6', KEY_6, 0),
    KEYCOMBO('7', KEY_7, 0),
    KEYCOMBO('8', KEY_8, 0),
    KEYCOMBO('9', KEY_9, 0),
    KEYCOMBO(':', KEY_SEMICOLON, MOD_LEFT_SHIFT),
    KEYCOMBO(';', KEY_SEMICOLON, 0),
    KEYCOMBO('<', KEY_COMMA, MOD_LEFT_SHIFT),
    KEYCOMBO('=', KEY_EQUAL, 0),
    KEYCOMBO('>', KEY_DOT, MOD_LEFT_SHIFT),
    KEYCOMBO('?', KEY_SLASH, MOD_LEFT_SHIFT),
    KEYCOMBO('@', KEY_2, MOD_LEFT_SHIFT),
    KEYCOMBO('A', KEY_A, MOD_LEFT_SHIFT),
    KEYCOMBO('B', KEY_B, MOD_LEFT_SHIFT),
    KEYCOMBO('C', KEY_C, MOD_LEFT_SHIFT),
    KEYCOMBO('D', KEY_D, MOD_LEFT_SHIFT),
    KEYCOMBO('E', KEY_E, MOD_LEFT_SHIFT),
    KEYCOMBO('F', KEY_F, MOD_LEFT_SHIFT),
    KEYCOMBO('G', KEY_G, MOD_LEFT_SHIFT),
    KEYCOMBO('H', KEY_H, MOD_LEFT_SHIFT),
    KEYCOMBO('I', KEY_I, MOD_LEFT_SHIFT),
    KEYCOMBO('J', KEY_J, MOD_LEFT_SHIFT),
    KEYCOMBO('K', KEY_K, MOD_LEFT_SHIFT),
    KEYCOMBO('L', KEY_L, MOD_LEFT_SHIFT),
    KEYCOMBO('M', KEY_M, MOD_LEFT_SHIFT),
    KEYCOMBO('N', KEY_N, MOD_LEFT_SHIFT),
    KEYCOMBO('O', KEY_O, MOD_LEFT_SHIFT),
    KEYCOMBO('P', KEY_P, MOD_LEFT_SHIFT),
    KEYCOMBO('Q', KEY_Q, MOD_LEFT_SHIFT),
    KEYCOMBO('R', KEY_R, MOD_LEFT_SHIFT),
    KEYCOMBO('S', KEY_S, MOD_LEFT_SHIFT),
    KEYCOMBO('T', KEY_T, MOD_LEFT_SHIFT),
    KEYCOMBO('U', KEY_U, MOD_LEFT_SHIFT),
    KEYCOMBO('V', KEY_V, MOD_LEFT_SHIFT),
    KEYCOMBO('W', KEY_W, MOD_LEFT_SHIFT),
    KEYCOMBO('X', KEY_X, MOD_LEFT_SHIFT),
    KEYCOMBO('Y', KEY_Y, MOD_LEFT_SHIFT),
    KEYCOMBO('Z', KEY_Z, MOD_LEFT_SHIFT),
    KEYCOMBO('[', KEY_LEFTBRACE, 0),
    KEYCOMBO('\\', KEY_BACKSLASH, 0),
    KEYCOMBO(']', KEY_RIGHTBRACE, 0),
    KEYCOMBO('^', KEY_6, MOD_LEFT_SHIFT),
    KEYCOMBO('_', KEY_MINUS, MOD_LEFT_SHIFT),
    KEYCOMBO('`', KEY_GRAVE, 0),
    KEYCOMBO('a', KEY_A, 0),
    KEYCOMBO('b', KEY_B, 0),
    KEYCOMBO('c', KEY_C, 0),
    KEYCOMBO('d', KEY_D, 0),
    KEYCOMBO('e', KEY_E, 0),
    KEYCOMBO('f', KEY_F, 0),
    KEYCOMBO('g', KEY_G, 0),
    KEYCOMBO('h', KEY_H, 0),
    KEYCOMBO('i', KEY_I, 0),
    KEYCOMBO('j', KEY_J, 0),
    KEYCOMBO('k', KEY_K, 0),
    KEYCOMBO('l', KEY_L, 0),
    KEYCOMBO('m', KEY_M, 0),
    KEYCOMBO('n', KEY_N, 0),
    KEYCOMBO('o', KEY_O, 0),
    KEYCOMBO('p', KEY_P, 0),
    KEYCOMBO('q', KEY_Q, 0),
    KEYCOMBO('r', KEY_R, 0),
    KEYCOMBO('s', KEY_S, 0),
    KEYCOMBO('t', KEY_T, 0),
    KEYCOMBO('u', KEY_U, 0),
    KEYCOMBO('v', KEY_V, 0),
    KEYCOMBO('w', KEY_W, 0),
    KEYCOMBO('x', KEY_X, 0),
    KEYCOMBO('y', KEY_Y, 0),
    KEYCOMBO('z', KEY_Z, 0),
    KEYCOMBO('{', KEY_LEFTBRACE, MOD_LEFT_SHIFT),
    KEYCOMBO('|', KEY_BACKSLASH, MOD_LEFT_SHIFT),
    KEYCOMBO('}', KEY_RIGHTBRACE, MOD_LEFT_SHIFT),
    KEYCOMBO('~', KEY_GRAVE, MOD_LEFT_SHIFT),
};

int* get_modifier_keys(struct key_combination combo, int* size) {
    unsigned int bitmask = combo.modifiers;
    *size = 0;
    int* modifiers = NULL;
    while (bitmask) {
        modifiers = (int*)realloc(modifiers, sizeof(int) * (1 + *size));
        modifiers[(*size)++] = MOD_KEYS[__builtin_ctz(bitmask)];
        bitmask &= bitmask - 1;
    }
    return modifiers;
}

void send_key_event(struct libevdev_uinput* uinput_dev, int keycode, int value) {
    libevdev_uinput_write_event(uinput_dev, EV_KEY, keycode, value);
    libevdev_uinput_write_event(uinput_dev, EV_SYN, SYN_REPORT, 0);
}

void send_ascii_char(struct libevdev_uinput* uinput_dev, char c) {
    if (c < 0) {
        printf("Char %c unsupported\n", c);
        return;
    }
    struct key_combination kc = char_map[(int)c];
    if (kc.keycode == '\0') {
        printf("Char %c unsupported\n", c);
        return;
    }
    int modifier_size = 0;
    int* modifiers = get_modifier_keys(kc, &modifier_size);
    for (int i = 0; i < modifier_size; i++) {
        send_key_event(uinput_dev, modifiers[i], 1);
    }
    send_key_event(uinput_dev, kc.keycode, 1);
    send_key_event(uinput_dev, kc.keycode, 0);
    for (int i = 0; i < modifier_size; i++) {
        send_key_event(uinput_dev, modifiers[i], 0);
    }
    free(modifiers);
}

void send_string(struct libevdev_uinput* uinput_dev, const char* str) {
    while(*str) {
        send_ascii_char(uinput_dev, *str);
        str++;
    }
}