#include "dict.h"
#include "unity.h"
#include <stdlib.h>

void setUp(void) {

}

void tearDown(void) {

}

void clvrDictInit_should_returnStructInstancePointer(void) {
    clover_dict* dict = clover_dict_init(0, NULL);
    TEST_ASSERT_EQUAL(0, dict->id);
    TEST_ASSERT_EQUAL(NULL, dict->parent);
    clover_dict_free(dict);
}

void clvrDictGet_should_returnContainedStruct_orNull(void) {
    clover_dict* dict = clover_dict_init(0, NULL);
    clover_chord ids[2] = { 1U, 0 };
    clover_dict_add_entry(dict, ids, "=repeat_last_translation");

    clover_dict* response = clover_dict_get(dict, 3U);
    TEST_ASSERT_NULL(response);

    response = clover_dict_get(dict, 1U);
    TEST_ASSERT_NOT_NULL(response);
    clover_dict_free(dict);
}

void clvrDictSeek_should_recursivelyFindTargetDict(void) {
    clover_dict* dict = clover_dict_init(0, NULL);
    size_t buffer_len = 512;
    clover_chord buffer [buffer_len];
    clover_chord* ids = clover_parse_compound_chord(buffer, buffer_len, "KWHEFRT/TKA*EU/-Z");
    clover_dict_add_entry(dict, ids, "yesterdays");
    clover_dict* subdict = clover_dict_seek(dict, ids);
    
    
    TEST_ASSERT_EQUAL(clover_parse_chord("-Z"), subdict->id);
    TEST_ASSERT_EQUAL(clover_parse_chord("TKA*EU"), subdict->parent->id);
    TEST_ASSERT_EQUAL(1, subdict->translations->length);
    TEST_ASSERT_EQUAL_STRING("yesterdays", subdict->translations->head->value);
    TEST_ASSERT_EQUAL(0, subdict->children->size);
    clover_dict_free(dict);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(clvrDictInit_should_returnStructInstancePointer);
    RUN_TEST(clvrDictGet_should_returnContainedStruct_orNull);
    RUN_TEST(clvrDictSeek_should_recursivelyFindTargetDict);
    return UNITY_END();
}
