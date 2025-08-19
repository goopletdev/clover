#include "../clover-json/clover-json.h"
#include "./Unity/src/unity.h"

void setUp(void) {

}

void tearDown(void) {

}

void clvrParseDict_shouldParseFromJsonFile(void) {
    char file_path[] = "../assets/test-dictionary.json";
    clover_dict* dict = clover_parse_dictionary(file_path);

    TEST_ASSERT_EQUAL(24, clover_dict_size(dict));
    char* translation = clover_dict_translation(
        clover_get(dict, clover_parse_chord("#"))
    );
    TEST_ASSERT_EQUAL_STRING("=repeat_last_translation", translation);
    translation = clover_dict_translation(
        clover_get(dict, clover_parse_chord("#SH*"))
    );
    TEST_ASSERT_EQUAL_STRING("{^}.sh", translation);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(clvrParseDict_shouldParseFromJsonFile);
    return UNITY_END();
}