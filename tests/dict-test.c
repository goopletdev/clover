#include "dict.h"
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
    clover_free_dict(dict);
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
    clover_free_dict(dict);
}

void clvrSeek_shouldRecursivelyFindTargetDict(void) {
    clover_dict* dict = clover_table_init_dict(0, NULL);
    int size = 0;
    clover_chord* ids = clover_parse_compound_chord("KWHEFRT/TKA*EU/-Z", &size);
    clover_dict_push_entry(dict, ids, size, "yesterdays");
    clover_dict* subdict = clover_seek(dict, ids, size);
    TEST_ASSERT_EQUAL(clover_parse_chord("-Z"), subdict->id);
    TEST_ASSERT_EQUAL(clover_parse_chord("TKA*EU"), subdict->parent->id);
    TEST_ASSERT_EQUAL(1, subdict->translations.len);
    TEST_ASSERT_EQUAL_STRING("yesterdays", subdict->translations.entries[0]);
    TEST_ASSERT_EQUAL(0, subdict->children.size);
    clover_free_dict(dict);
    free(ids);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(clvrInitDict_shouldReturnStructInstancePointer);
    RUN_TEST(clvrDictGet_shouldReturnContainedStruct_orNull);
    RUN_TEST(clvrSeek_shouldRecursivelyFindTargetDict);
    return UNITY_END();
}
