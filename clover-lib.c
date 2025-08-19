#include "./clvrlib.h"
#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const unsigned int QWERTY_KEY_VALS[KEY_MAX + 1] = {
    [0 ... KEY_MAX] = 0,

    [KEY_Q] = 1 << 0, [KEY_A] = 1 << 1, 
    [KEY_W] = 1 << 2, [KEY_S] = 1 << 3,
    [KEY_E] = 1 << 4, [KEY_D] = 1 << 5, 
    [KEY_R] = 1 << 6, [KEY_F] = 1 << 7,

    [KEY_C] = 1 << 8, [KEY_V] = 1 << 9,
    [KEY_T] = 1 << 10, [KEY_G] = 1 << 10, 
    [KEY_Y] = 1 << 10, [KEY_H] = 1 << 10,
    [KEY_N] = 1 << 11, [KEY_M] = 1 << 12,

    [KEY_U] = 1 << 13, [KEY_J] = 1 << 14, 
    [KEY_I] = 1 << 15, [KEY_K] = 1 << 16,
    [KEY_O] = 1 << 17, [KEY_L] = 1 << 18, 
    [KEY_P] = 1 << 19, [KEY_SEMICOLON] = 1 << 20,
    [KEY_LEFTBRACE] = 1 << 21, [KEY_APOSTROPHE] = 1 << 22
};