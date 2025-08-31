#include "json.h"
#include "unity.h"

void setUp(void) {

}

void tearDown(void) {

}

void clvrParseDict_shouldParseFromJsonFile(void) {
    char file_path[] = "dictionaries/test-dictionary.json";
    clover_dict* dict = clover_table_init_dict(0, NULL);
    dict = clover_parse_dictionary(file_path, dict);

    TEST_ASSERT_EQUAL(24, dict->children.size);
    const char* translation = clover_dict_get(dict, clover_parse_chord("#"))->translations.entries[0];
    TEST_ASSERT_EQUAL_STRING("=repeat_last_translation", translation);
    translation = clover_dict_get(dict, clover_parse_chord("#SH*"))->translations.entries[0];
    TEST_ASSERT_EQUAL_STRING("{^}.sh", translation);
}

void clvrParseDict_shouldParseMultipleJsonsToSameObject(void) {
    char file_path[] = "dictionaries/test-dictionary.json";
    clover_dict* dict = clover_table_init_dict(0, NULL);
    dict = clover_parse_dictionary(file_path, dict);

    char file_path_2[] = "dictionaries/test-dictionary-2.json";
    dict = clover_parse_dictionary(file_path_2, dict);

    TEST_ASSERT_EQUAL(29, dict->children.size);

    printf("Testing unchanged chords after parsing second dictionary\n");
    const char* translation = clover_dict_get(dict, clover_parse_chord("#"))->translations.entries[0];
    TEST_ASSERT_EQUAL_STRING("=repeat_last_translation", translation);

    translation = clover_dict_get(dict, clover_parse_chord("#SH*"))->translations.entries[0];
    TEST_ASSERT_EQUAL_STRING("{^}.sh", translation);

    printf("Testing new chords after parsing second dictionary...\n");

    translation = clover_dict_get(dict, clover_parse_chord("STKPWRAOE"))->translations.entries[0];
    TEST_ASSERT_EQUAL_STRING("disagree", translation);

    printf("Testing collisions...\n");
    translation = clover_dict_get(dict, clover_parse_chord("ST"))->translations.entries[0];
    TEST_ASSERT_EQUAL_STRING("is it", translation);

    int size = 0;
    clover_dict* subdict = clover_seek(dict, clover_parse_compound_chord("STKPW/TAO*EUP", &size), size);
    translation = subdict->translations.entries[0];
    printf("Translation: %s\n", translation);
    printf("size value: %i\n", size);
    TEST_ASSERT_EQUAL_STRING("ztype", translation);

    printf("Testing collisions...\n");
    translation = clover_dict_get(dict, clover_parse_chord("ST"))->translations.entries[0];
    TEST_ASSERT_EQUAL_STRING("is it", translation);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(clvrParseDict_shouldParseFromJsonFile);
    //RUN_TEST(clvrParseDict_shouldParseMultipleJsonsToSameObject);
    return UNITY_END();
}
