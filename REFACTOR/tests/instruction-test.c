#include "instruction.h"
#include "unity.h"

clover_instance* test_instance;
clover_instruction_node* test_macro_inst[8];

void setUp(void) {
    test_instance = clover_instance_init();
}

void tearDown(void) {
    clover_instance_free(test_instance);
}

void clvrInstructionInstanceTriesInitialization_shouldHaveDefaultMacros(void) {
    test_macro_inst[0] = clover_instruction_from_macro("retro_insert_space", test_instance->macros);
    test_macro_inst[1] = clover_instruction_from_macro("retrospective_insert_space", test_instance->macros);
    test_macro_inst[2] = clover_instruction_from_macro("retro_delete_space", test_instance->macros);
    test_macro_inst[3] = clover_instruction_from_macro("retrospective_delete_space", test_instance->macros);
    test_macro_inst[4] = clover_instruction_from_macro("undo", test_instance->macros);
    test_macro_inst[5] = clover_instruction_from_macro("repeat_last_stroke", test_instance->macros);
    test_macro_inst[6] = clover_instruction_from_macro("retro_toggle_asterisk", test_instance->macros);
    test_macro_inst[7] = clover_instruction_from_macro("retrospective_toggle_asterisk", test_instance->macros);

    for (int i = 0; i < 8; i++) {
        TEST_ASSERT_NOT_NULL(test_macro_inst[i]);
    }

    TEST_ASSERT_EQUAL(1, test_macro_inst[0]->u.macro);
    TEST_ASSERT_EQUAL(1, test_macro_inst[1]->u.macro);
    TEST_ASSERT_EQUAL(2, test_macro_inst[2]->u.macro);
    TEST_ASSERT_EQUAL(2, test_macro_inst[3]->u.macro);
    TEST_ASSERT_EQUAL(3, test_macro_inst[4]->u.macro);
    TEST_ASSERT_EQUAL(4, test_macro_inst[5]->u.macro);
    TEST_ASSERT_EQUAL(5, test_macro_inst[6]->u.macro);
    TEST_ASSERT_EQUAL(5, test_macro_inst[7]->u.macro);

    for (int i = 0; i < 8; i++) {
        clover_instruction_free_node(test_macro_inst[i]);
    }
}

void clvrInstruction_fromBrackets_shouldSetType(void) {

}

int main(void) {    
    UNITY_BEGIN();
    RUN_TEST(clvrInstructionInstanceTriesInitialization_shouldHaveDefaultMacros);
    RUN_TEST(clvrInstruction_fromBrackets_shouldSetType);
    return UNITY_END();
}
