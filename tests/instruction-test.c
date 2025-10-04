#include "instruction.h"
#include "unity.h"

void setUp(void) {
    clover_instruction_start();
}

void tearDown(void) {
    clover_instruction_cleanup();
}

void clvrInstruction_testLookupMacroOrCommand(void) {
    TEST_ASSERT_EQUAL(RETRO_INSERT_SPACE, clover_instruction_lookup_macro("retro_insert_space"));
    TEST_ASSERT_EQUAL(RETRO_INSERT_SPACE, clover_instruction_lookup_macro("retrospective_insert_space"));
    TEST_ASSERT_EQUAL(RETRO_DELETE_SPACE, clover_instruction_lookup_macro("retro_delete_space"));
    TEST_ASSERT_EQUAL(RETRO_DELETE_SPACE, clover_instruction_lookup_macro("retrospective_delete_space"));
    TEST_ASSERT_EQUAL(UNDO, clover_instruction_lookup_macro("undo"));
    TEST_ASSERT_EQUAL(REPEAT_LAST_STROKE, clover_instruction_lookup_macro("repeat_last_stroke"));
    TEST_ASSERT_EQUAL(RETRO_TOGGLE_ASTERISK, clover_instruction_lookup_macro("retro_toggle_asterisk"));
    TEST_ASSERT_EQUAL(RETRO_TOGGLE_ASTERISK, clover_instruction_lookup_macro("retrospective_toggle_asterisk"));
}

void clvrInstruction_fromBrackets_shouldSetType(void) {

}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(clvrInstruction_testLookupMacroOrCommand);
    RUN_TEST(clvrInstruction_fromBrackets_shouldSetType);
    return UNITY_END();
}
