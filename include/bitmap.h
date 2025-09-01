#ifndef CLOVER_BITMAP_H
#define CLOVER_BITMAP_H

#include <limits.h>
#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>

#define BITS_PER_LONG (sizeof(unsigned long) * CHAR_BIT)
#define CLOVER_TEST_BIT(bit, array) ((array)[(bit)/BITS_PER_LONG]) & (1UL << ((bit)%BITS_PER_LONG))
#define CLOVER_SET_BIT(bit, array) ((array)[(bit)/BITS_PER_LONG]) &= (1UL << ((bit)%BITS_PER_LONG))
#define CLOVER_BITMAP_LENGTH ((KEY_MAX / BITS_PER_LONG) + 1)

typedef unsigned long clover_bitmap[(KEY_MAX / BITS_PER_LONG) + 1];

#endif // CLOVER_BITMAP_H
