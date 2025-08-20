#include "../clover-hash/clover-hash.h"
#include "./Unity/src/unity.h"
#include <stdlib.h>

void setUp(void) {

}

void tearDown(void) {

}


void clvrInitDict_shouldReturnStructInstancePointer(void) {
    clover_dict* dict = clover_init_dict(0, NULL, NULL);
    TEST_ASSERT_EQUAL(0, clover_dict_id(dict));
    TEST_ASSERT_EQUAL(NULL, clover_dict_parent(dict));
}

void clvrHas_shouldBeTrueIfHas_shouldBeFalseIfNot(void) {
    clover_dict* dict = clover_init_dict(0, NULL, NULL);
    unsigned int ids[0];
    ids[0] = 1U;
    clover_push_entry(dict, ids, 1, "=repeat_last_translation");

    TEST_ASSERT_FALSE(clover_has(dict, 2U));
    TEST_ASSERT_TRUE(clover_has(dict, 1U));
}


void clvrGet_shouldReturnContainedStruct_orNull(void) {
    clover_dict* dict = clover_init_dict(0, NULL, NULL);
    unsigned int ids[0];
    ids[0] = 1U;
    clover_push_entry(dict, ids, 1, "=repeat_last_translation");

    clover_dict* response = clover_get(dict, 3U);
    TEST_ASSERT_NULL(response);

    response = clover_get(dict, 1U);
    TEST_ASSERT_NOT_NULL(response);
}

void clvrPushEntry_shouldRecursivelyAddEntries(void) {
    clover_dict* dict = clover_init_dict(0, NULL, NULL);
    unsigned int ids[3] = {
        [0] = clover_parse_chord("KWHEFRT"),
        [1] = clover_parse_chord("TKA*EU"),
        [2] = clover_parse_chord("-Z")
    };
    char* translation = "yesterdays";
    clover_push_entry(dict, ids, 3, translation);

    TEST_ASSERT_EQUAL(1, clover_dict_size(dict));
    dict = clover_get(dict, ids[0]);
    TEST_ASSERT_EQUAL(1, clover_dict_size(dict));
    dict = clover_get(dict, ids[1]);
    TEST_ASSERT_EQUAL(1, clover_dict_size(dict));
    dict = clover_get(dict, ids[2]);
    TEST_ASSERT_EQUAL(0, clover_dict_size(dict));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(clvrHas_shouldBeTrueIfHas_shouldBeFalseIfNot);
    RUN_TEST(clvrInitDict_shouldReturnStructInstancePointer);
    RUN_TEST(clvrGet_shouldReturnContainedStruct_orNull);
    RUN_TEST(clvrPushEntry_shouldRecursivelyAddEntries);
    return UNITY_END();
}