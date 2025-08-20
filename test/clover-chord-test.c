#include "../clover-chord/clover-chord.h"
#include "./Unity/src/unity.h"

void setUp(void) {

}

void tearDown(void) {
    
}

void clvrChordSize_shouldCountHighBits_ignoringFlags(void) {
    TEST_ASSERT_EQUAL(3, clover_chord_size(7U));
    TEST_ASSERT_EQUAL(3, clover_chord_size(7U | (1U << 23)));
}

void clvrPrettyChord_shouldEncodeUInts_withHyphen(void) {
    TEST_ASSERT_EQUAL_STRING("#", clover_pretty_chord(1U));
    TEST_ASSERT_EQUAL_STRING("#KRO*PL", clover_pretty_chord(165513U));
    TEST_ASSERT_EQUAL_STRING("-F", clover_pretty_chord(8192U));
    TEST_ASSERT_EQUAL_STRING("TP-PL", clover_pretty_chord(163860U));
}

void clvrPaperTape_shouldEncodeUInts(void) {
    TEST_ASSERT_EQUAL_STRING("#                      ", clover_paper_tape(1U));
    TEST_ASSERT_EQUAL_STRING("#  K   R O*    P L     ", clover_paper_tape(165513U));
    TEST_ASSERT_EQUAL_STRING("             F         ", clover_paper_tape(8192U));
    TEST_ASSERT_EQUAL_STRING("  T P          P L     ", clover_paper_tape(163860U));
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

void clvrParseCompoundChord_shouldHandleArbitraryChordSizes(void) {
    int size = 0;
    unsigned int* chord; 
    char* key = "STKPWAO/STKPWAO/HRO/SKWREUBG/KWRAL";
    chord = clover_parse_compound_chord(key, &size);
    TEST_ASSERT_EQUAL(5, size);
    TEST_ASSERT_EQUAL(clover_parse_chord("STKPWAO"), chord[0]);

    size = 0;
    key = "#";
    chord = clover_parse_compound_chord(key, &size);
    TEST_ASSERT_EQUAL(1, size);
    TEST_ASSERT_EQUAL(clover_parse_chord("#"), chord[0]);

    size = 0;
    key = "SRE/TER/KWREU/TPHAEUR/KWREU/KWRAPB/-Z";
    chord = clover_parse_compound_chord(key, &size);
    TEST_ASSERT_EQUAL(7, size);
    TEST_ASSERT_EQUAL(clover_parse_chord("SRE"), chord[0]);
    TEST_ASSERT_EQUAL(clover_parse_chord("TER"), chord[1]);
    TEST_ASSERT_EQUAL(clover_parse_chord("KWREU"), chord[2]);
    TEST_ASSERT_EQUAL(clover_parse_chord("TPHAEUR"), chord[3]);
    TEST_ASSERT_EQUAL(clover_parse_chord("KWREU"), chord[4]);
    TEST_ASSERT_EQUAL(clover_parse_chord("KWRAPB"), chord[5]);
    TEST_ASSERT_EQUAL(clover_parse_chord("-Z"), chord[6]);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(clvrChordSize_shouldCountHighBits_ignoringFlags);
    RUN_TEST(clvrPrettyChord_shouldEncodeUInts_withHyphen);
    RUN_TEST(clvrPaperTape_shouldEncodeUInts);
    RUN_TEST(clover_chord_compare_should_observe_steno_order);
    RUN_TEST(clvrParseChord_shouldParseToUInt);
    RUN_TEST(clvrParseCompoundChord_shouldHandleArbitraryChordSizes);
    return UNITY_END();
}