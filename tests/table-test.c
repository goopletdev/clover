#include "table.h"
#include "unity.h"
#include <stdlib.h>

void setUp(void) {

}

void tearDown(void) {

}

void clvrInitDict_shouldReturnStructInstancePointer(void) {
    clover_dict* dict = clover_table_init_dict(0, NULL);
    TEST_ASSERT_EQUAL(0, dict->id);
    TEST_ASSERT_EQUAL(NULL, dict->parent);
}

void clvrDictGet_shouldReturnContainedStruct_orNull(void) {
    clover_dict* dict = clover_table_init_dict(0, NULL);
    clover_chord ids[1];
    ids[0] = 1U;
    clover_dict_push_entry(dict, ids, 1, "=repeat_last_translation");

    clover_dict* response = clover_dict_get(dict, 3U);
    TEST_ASSERT_NULL(response);

    response = clover_dict_get(dict, 1U);
    TEST_ASSERT_NOT_NULL(response);
}

void clvrPushEntry_shouldRecursivelyAddEntries(void) {
    clover_dict* dict = clover_table_init_dict(0, NULL);
    clover_chord ids[3] = {
        [0] = clover_parse_chord("KWHEFRT"),
        [1] = clover_parse_chord("TKA*EU"),
        [2] = clover_parse_chord("-Z")
    };
    char* translation = "yesterdays";
    clover_dict_push_entry(dict, ids, 3, translation);
    TEST_ASSERT_EQUAL(1, dict->children.size);
    dict = clover_dict_get(dict, ids[0]);
    TEST_ASSERT_EQUAL(1, dict->children.size);
    dict = clover_dict_get(dict, ids[1]);
    TEST_ASSERT_EQUAL(1, dict->children.size);
    dict = clover_dict_get(dict, ids[2]);
    TEST_ASSERT_EQUAL(0, dict->children.size);
}

void clvrSeek_shouldRecursivelyFindTargetDict(void) {
    clover_dict* dict = clover_table_init_dict(0, NULL);
    int size = 0;
    clover_chord* ids = clover_parse_compound_chord("KWHEFRT/TKA*EU/-Z", &size);
    clover_dict_push_entry(dict, ids, size, "yesterdays");
    dict = clover_seek(dict, ids, size);
    TEST_ASSERT_EQUAL(clover_parse_chord("-Z"), dict->id);
    TEST_ASSERT_EQUAL(clover_parse_chord("TKA*EU"), dict->parent->id);
    TEST_ASSERT_EQUAL(1, dict->translations.len);
    TEST_ASSERT_EQUAL_STRING("yesterdays", dict->translations.entries[0]);
    TEST_ASSERT_EQUAL(0, dict->children.size);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(clvrInitDict_shouldReturnStructInstancePointer);
    RUN_TEST(clvrDictGet_shouldReturnContainedStruct_orNull);
    RUN_TEST(clvrPushEntry_shouldRecursivelyAddEntries);
    RUN_TEST(clvrSeek_shouldRecursivelyFindTargetDict);
    return UNITY_END();
}
