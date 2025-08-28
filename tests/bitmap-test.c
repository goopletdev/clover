#include "bitmap.h"
#include "unity.h"

// for input key values
#include <linux/input.h>

void setUp(void) {

}

void tearDown(void) {

}

void clvrParseHexmap_shouldParseArbitraryHexmaps(void) {
    const char* h = "1000000000007 ff98007a000007ff febeffdfffefffff fffffffffffffffe\n";
    clover_bitmap m = clover_parse_hexmap(h);
    TEST_ASSERT_EQUAL(4, m.len);
    TEST_ASSERT_EQUAL(0UL-2UL, m.words[0]);
    clover_free_bitmap(m);
}

void clvrInitEmptyBitmap_shouldInitializeToZeroes(void) {
    clover_bitmap m = clover_init_empty_bitmap(4);
    TEST_ASSERT_EQUAL(4, m.len);
    TEST_ASSERT_EQUAL(0UL, m.words[0]);
    TEST_ASSERT_EQUAL(0UL, m.words[1]);
    TEST_ASSERT_EQUAL(0UL, m.words[2]);
    TEST_ASSERT_EQUAL(0UL, m.words[3]);
    clover_free_bitmap(m);
}

void clvrBitmapAddValue_shouldRecursivelyLengthenBitmap(void) {
    clover_bitmap m = clover_init_empty_bitmap(0);
    TEST_ASSERT_EQUAL(0, m.len);
    m = clover_bitmap_add_value(m, 65);
    TEST_ASSERT_EQUAL(2, m.len);
    TEST_ASSERT_EQUAL(0UL, m.words[0]);
    TEST_ASSERT_EQUAL(2UL, m.words[1]);
    clover_free_bitmap(m);
}

void clvrBitmapFromArray_shouldMakeBitmapFromInts(void) {
    const int REQUIRED_KEYS[] = {
        KEY_Q, KEY_A, KEY_W, KEY_S, KEY_E, KEY_D, KEY_R, KEY_F,
        KEY_C, KEY_V, KEY_T, KEY_G, KEY_Y, KEY_H, KEY_N, KEY_M,
        KEY_U, KEY_J, KEY_I, KEY_K, KEY_O, KEY_L, KEY_P,
        KEY_SEMICOLON, KEY_LEFTBRACE, KEY_APOSTROPHE,
    };
    clover_bitmap m = clover_bitmap_from_array(REQUIRED_KEYS, 26);
    TEST_ASSERT_EQUAL(1, m.len);
    TEST_ASSERT_EQUAL(1902154176462848UL, m.words[0]);
    clover_free_bitmap(m);
}

void clvrBitmapBitIsHigh_BitSetHigh_ClearBit_shouldBehave(void) {
    const int REQUIRED_KEYS[] = {
        KEY_Q, KEY_A, KEY_W, KEY_S, KEY_E, KEY_D, KEY_R, KEY_F,
        KEY_C, KEY_V, KEY_T, KEY_G, KEY_Y, KEY_H, KEY_N, KEY_M,
        KEY_U, KEY_J, KEY_I, KEY_K, KEY_O, KEY_L, KEY_P,
        KEY_SEMICOLON, KEY_LEFTBRACE, KEY_APOSTROPHE,
    };
    clover_bitmap m = clover_bitmap_from_array(REQUIRED_KEYS, 26);
    TEST_ASSERT_EQUAL(1, clover_bitmap_bit_is_high(m, KEY_A));
    TEST_ASSERT_EQUAL(0, clover_bitmap_bit_is_high(m, KEY_Z));
    TEST_ASSERT_EQUAL(0, clover_bitmap_bit_is_high(m, 500));
    m = clover_bitmap_set_bit_high(m, KEY_Z);
    TEST_ASSERT_EQUAL(1, clover_bitmap_bit_is_high(m, KEY_Z));
    m = clover_bitmap_clear_bit(m, KEY_A);
    TEST_ASSERT_EQUAL(0, clover_bitmap_bit_is_high(m, KEY_A));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(clvrParseHexmap_shouldParseArbitraryHexmaps);
    RUN_TEST(clvrInitEmptyBitmap_shouldInitializeToZeroes);
    RUN_TEST(clvrBitmapAddValue_shouldRecursivelyLengthenBitmap);
    RUN_TEST(clvrBitmapFromArray_shouldMakeBitmapFromInts);
    RUN_TEST(clvrBitmapBitIsHigh_BitSetHigh_ClearBit_shouldBehave);
    return UNITY_END();
}
