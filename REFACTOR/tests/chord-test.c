#include "chord.h"
#include "unity.h"

#include <string.h> // memset

char char_buffer[512];
clover_chord chord_buffer[512];

void setUp(void) {
    memset(char_buffer, 0, 512 * sizeof(char));
    memset(chord_buffer, 0, 512 * sizeof(clover_chord));
}

void tearDown(void) {
    
}

void clvrChordSize_shouldCountHighBits_ignoringFlags(void) {
    TEST_ASSERT_EQUAL(3, clover_chord_size(7U));
    TEST_ASSERT_EQUAL(3, clover_chord_size(7U | (1U << 23)));
}

void clvrPrettyChord_shouldEncodeUInts_withHyphen(void) {
    clover_pretty_chord(char_buffer, 512, 1U);
    TEST_ASSERT_EQUAL_STRING("#", char_buffer);

    clover_pretty_chord(char_buffer, 512, 165513U);
    TEST_ASSERT_EQUAL_STRING("#KRO*PL", char_buffer);

    clover_pretty_chord(char_buffer, 512, 8192U);
    TEST_ASSERT_EQUAL_STRING("-F", char_buffer);

    clover_pretty_chord(char_buffer, 512, 163860U);
    TEST_ASSERT_EQUAL_STRING("TP-PL", char_buffer);

    clover_pretty_chord(char_buffer, 512, 1091U);
    TEST_ASSERT_EQUAL_STRING("#SH*", char_buffer);
}

void clvrPaperTape_shouldEncodeUInts(void) {
    clover_paper_tape(char_buffer, 512, 1U);
    TEST_ASSERT_EQUAL_STRING("#                      ", char_buffer);
    
    clover_paper_tape(char_buffer, 512, 165513U);
    TEST_ASSERT_EQUAL_STRING("#  K   R O*    P L     ", char_buffer);

    clover_paper_tape(char_buffer, 512, 8192U);
    TEST_ASSERT_EQUAL_STRING("             F         ", char_buffer);

    clover_paper_tape(char_buffer, 512, 163860U);
    TEST_ASSERT_EQUAL_STRING("  T P          P L     ", char_buffer);
}

void clover_chord_compare_should_observe_steno_order(void) {
    TEST_ASSERT_EQUAL(0, clover_chord_compare(165513U, 165513U));
    TEST_ASSERT_EQUAL(-1, clover_chord_compare(1U, 165513U));
    TEST_ASSERT_EQUAL(-1, clover_chord_compare(1U, 8192U));
    TEST_ASSERT_EQUAL(1, clover_chord_compare(163860U, 165513U));
}

void clvrParseChord_shouldParseToUInt(void) {
    TEST_ASSERT_EQUAL(1U, clover_parse_chord("#"));
    TEST_ASSERT_EQUAL(165513U, clover_parse_chord("#KRO*PL"));
    TEST_ASSERT_EQUAL(8192U, clover_parse_chord("-F"));
    TEST_ASSERT_EQUAL(163860U, clover_parse_chord("TP-PL"));
    TEST_ASSERT_EQUAL(4194304U, clover_parse_chord("-Z"));
}

void clvrChordLen_shouldCountNumberOfStrokes(void) {
    TEST_ASSERT_EQUAL(0, clover_chord_len(chord_buffer));
    chord_buffer[0] = 1U;
    TEST_ASSERT_EQUAL(1, clover_chord_len(chord_buffer));
    chord_buffer[1] = 1U;
    TEST_ASSERT_EQUAL(2, clover_chord_len(chord_buffer));
}

void clvrParseCompoundChord_shouldHandleArbitraryChordSizes(void) {
    char* key = "STKPWAO/STKPWAO/HRO/SKWREUBG/KWRAL";
    clover_parse_compound_chord(chord_buffer, 512, key);
    TEST_ASSERT_EQUAL(5, clover_chord_len(chord_buffer));
    TEST_ASSERT_EQUAL(clover_parse_chord("STKPWAO"), chord_buffer[0]);

    key = "#";
    clover_parse_compound_chord(chord_buffer, 512, key);
    TEST_ASSERT_EQUAL(1, clover_chord_len(chord_buffer));
    TEST_ASSERT_EQUAL(clover_parse_chord("#"), chord_buffer[0]);

    key = "SRE/TER/KWREU/TPHAEUR/KWREU/KWRAPB/-Z";
    clover_parse_compound_chord(chord_buffer, 512, key);
    TEST_ASSERT_EQUAL(7, clover_chord_len(chord_buffer));
    TEST_ASSERT_EQUAL(clover_parse_chord("SRE"), chord_buffer[0]);
    TEST_ASSERT_EQUAL(clover_parse_chord("TER"), chord_buffer[1]);
    TEST_ASSERT_EQUAL(clover_parse_chord("KWREU"), chord_buffer[2]);
    TEST_ASSERT_EQUAL(clover_parse_chord("TPHAEUR"), chord_buffer[3]);
    TEST_ASSERT_EQUAL(clover_parse_chord("KWREU"), chord_buffer[4]);
    TEST_ASSERT_EQUAL(clover_parse_chord("KWRAPB"), chord_buffer[5]);
    TEST_ASSERT_EQUAL(clover_parse_chord("-Z"), chord_buffer[6]);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(clvrChordSize_shouldCountHighBits_ignoringFlags);
    RUN_TEST(clvrPrettyChord_shouldEncodeUInts_withHyphen);
    RUN_TEST(clvrPaperTape_shouldEncodeUInts);
    RUN_TEST(clover_chord_compare_should_observe_steno_order);
    RUN_TEST(clvrParseChord_shouldParseToUInt);
    RUN_TEST(clvrChordLen_shouldCountNumberOfStrokes);
    RUN_TEST(clvrParseCompoundChord_shouldHandleArbitraryChordSizes);
    return UNITY_END();
}
