#include "json.h"
#include "unity.h"

void setUp(void) {

}

void tearDown(void) {

}

void clvrJsonParse_should_parseFromJsonFile(void) {
    char file_path[] = "dictionaries/test-dictionary.json";
    clover_dict* dict = clover_dict_init(0, NULL);
    clover_json_parse(file_path, dict);

    TEST_ASSERT_EQUAL(24, dict->children->size);
    const char* translation = clover_dict_get(dict, clover_parse_chord("#"))->translations->head->value;
    TEST_ASSERT_EQUAL_STRING("=repeat_last_translation", translation);
    translation = clover_dict_get(dict, clover_parse_chord("#SH*"))->translations->head->value;
    TEST_ASSERT_EQUAL_STRING("{^}.sh", translation);
    clover_dict_free(dict);
}

void clvrJsonParse_should_parseMultipleJsonsToSameObject(void) {
    char file_path[] = "dictionaries/test-dictionary.json";
    clover_dict* dict = clover_dict_init(0, NULL);
    clover_json_parse(file_path, dict);

    char file_path_2[] = "dictionaries/test-dictionary-2.json";
    clover_json_parse(file_path_2, dict);

    TEST_ASSERT_EQUAL(29, dict->children->size);

    const char* translation = clover_dict_get(dict, clover_parse_chord("#"))->translations->head->value;
    TEST_ASSERT_EQUAL_STRING("=repeat_last_translation", translation);

    translation = clover_dict_get(dict, clover_parse_chord("#SH*"))->translations->head->value;
    TEST_ASSERT_EQUAL_STRING("{^}.sh", translation);

    translation = clover_dict_get(dict, clover_parse_chord("STKPWRAOE"))->translations->head->value;
    TEST_ASSERT_EQUAL_STRING("disagree", translation);

    translation = clover_dict_get(dict, clover_parse_chord("ST"))->translations->head->value;
    TEST_ASSERT_EQUAL_STRING("is it", translation);
    TEST_ASSERT_EQUAL(2, clover_dict_get(dict, clover_parse_chord("ST"))->translations->length);

    clover_chord chord_buffer[512] = {0};
    clover_parse_compound_chord(chord_buffer, 512, "STKPW/TAO*EUP");
    clover_dict* subdict = clover_dict_seek(dict, chord_buffer);
    translation = subdict->translations->head->value;
    TEST_ASSERT_EQUAL_STRING("ztype", translation);

    translation = clover_dict_get(dict, clover_parse_chord("ST"))->translations->head->value;
    TEST_ASSERT_EQUAL_STRING("is it", translation);

    clover_dict_free(dict);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(clvrJsonParse_should_parseFromJsonFile);
    RUN_TEST(clvrJsonParse_should_parseMultipleJsonsToSameObject);
    return UNITY_END();
}
