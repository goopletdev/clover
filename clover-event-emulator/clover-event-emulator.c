#include <linux/input.h>
#include <stdio.h>

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
const unsigned int MOD_SUPER = 1U << 6;

static const struct key_combination char_map[128] = {
    [0 ... 127] = NULL,

    #define KEYCOMBO(x,y,z) [x] = { y, z }

    ['\b'] = { KEY_BACKSPACE, 0 },
    ['\t'] = { KEY_TAB, 0 },
    ['\n'] = { KEY_ENTER, 0 },

    [' '] = { KEY_SPACE, 0 },
    ['!'] = { KEY_1, MOD_RIGHT_SHIFT },
    ['"'] = { KEY_APOSTROPHE, MOD_LEFT_SHIFT },
    ['#'] = { KEY_3, MOD_RIGHT_SHIFT },
    KEYCOMBO('$', KEY_4, MOD_RIGHT_SHIFT),
    KEYCOMBO('%', KEY_5, MOD_RIGHT_SHIFT),
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



};