#include "history.h"
#include "unity.h"

void setUp(void) {

}

void tearDown(void) {

}

void clvrHistoryInit_shouldInitializeFields_toZeroOrNull(void) {
    clover_history* h = clover_history_init();
    TEST_ASSERT_NULL(h->head);
    TEST_ASSERT_NULL(h->tail);
    TEST_ASSERT_EQUAL(0, h->len);
    clover_history_free(h);
}

void clvrHistoryElementInit_shouldInitFields_toNull(void) {
    clover_history_element* e = clover_history_element_init();
    TEST_ASSERT_NULL(e->instruction);
    TEST_ASSERT_NULL(e->source_dict);
    TEST_ASSERT_NULL(e->prev);
    TEST_ASSERT_NULL(e->next);
    clover_history_free_element(e);
}

void clvrHistoryPush_shouldUpdateElements_prevAndNext(void) {
    clover_history* hist = clover_history_init();
    clover_history_element* e1 = clover_history_element_init();
    clover_history_element* e2 = clover_history_element_init();
    clover_history_element* e3 = clover_history_element_init();
    clover_history_element* e4 = clover_history_element_init();
    clover_history_push(hist, e1);
    clover_history_push(hist, e2);
    clover_history_push(hist, e3);

    clover_history_push(hist, e4);
    TEST_ASSERT_EQUAL(hist->head, e1);
    TEST_ASSERT_EQUAL(hist->tail, e4);
    TEST_ASSERT_EQUAL(4, hist->len);
    TEST_ASSERT_NULL(e1->prev);
    TEST_ASSERT_NULL(e4->next);
    TEST_ASSERT_EQUAL(e1->next, e2);
    TEST_ASSERT_EQUAL(e2->prev, e1);
    TEST_ASSERT_EQUAL(e3->prev, e2);
    TEST_ASSERT_EQUAL(e2->next, e3);
    TEST_ASSERT_EQUAL(e3->next, e4);
    TEST_ASSERT_EQUAL(e4->prev, e3);
    clover_history_free(hist);
}

void clvrHistoryPop_shouldUpdateElements_prevAndNext(void) {
    clover_history* hist = clover_history_init();
    clover_history_element* e1 = clover_history_element_init();
    clover_history_element* e2 = clover_history_element_init();
    clover_history_element* e3 = clover_history_element_init();
    clover_history_element* e4 = clover_history_element_init();
    clover_history_push(hist, e1);
    clover_history_push(hist, e2);
    clover_history_push(hist, e3);
    clover_history_push(hist, e4);
    TEST_ASSERT_EQUAL(clover_history_pop(hist), e4);
    TEST_ASSERT_EQUAL(3, hist->len);
    TEST_ASSERT_NULL(e4->prev);
    TEST_ASSERT_NULL(e4->next);
    TEST_ASSERT_NULL(e3->next);
    TEST_ASSERT_EQUAL(e3, hist->tail);
    clover_history_free_element(e4);
    clover_history_free(hist);
}

void clvrHistoryShift_shouldUpdateElements_prevAndNext(void) {
    clover_history* hist = clover_history_init();
    clover_history_element* e1 = clover_history_element_init();
    clover_history_element* e2 = clover_history_element_init();
    clover_history_element* e3 = clover_history_element_init();
    clover_history_element* e4 = clover_history_element_init();
    clover_history_push(hist, e1);
    clover_history_push(hist, e2);
    clover_history_push(hist, e3);
    clover_history_push(hist, e4);
    TEST_ASSERT_EQUAL(clover_history_shift(hist), e1);
    TEST_ASSERT_EQUAL(3, hist->len);
    TEST_ASSERT_NULL(e1->prev);
    TEST_ASSERT_NULL(e1->next);
    TEST_ASSERT_NULL(e2->prev);
    TEST_ASSERT_EQUAL(e2, hist->head);
    clover_history_free_element(e1);
    clover_history_free(hist);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(clvrHistoryInit_shouldInitializeFields_toZeroOrNull);
    RUN_TEST(clvrHistoryElementInit_shouldInitFields_toNull);
    RUN_TEST(clvrHistoryPush_shouldUpdateElements_prevAndNext);
    RUN_TEST(clvrHistoryPop_shouldUpdateElements_prevAndNext);
    RUN_TEST(clvrHistoryShift_shouldUpdateElements_prevAndNext);
    return UNITY_END();
}