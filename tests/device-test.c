#include "device.h"
#include "unity.h"

#include <stdlib.h> // for free()

void setUp(void) {

}

void tearDown(void) {

}

void clvrGetAllDevices_shouldGetAllSystemDevices(void) {
    clover_device_list dl = clover_get_all_devices();
    TEST_ASSERT_GREATER_THAN_INT(0, dl.len);
    for (int i = 0; i < dl.len; i++) {
        clover_free_device(dl.devices[i]);
    }
    free(dl.devices);
}

void clvrQueryUserForDevice_shouldReturnOneDevice(void) {
    clover_device_list dl = clover_get_all_devices();
    int len = dl.len;
    dl = clover_prune_devices(clover_device_is_keyboard, dl);
    TEST_ASSERT_LESS_THAN_INT(len, dl.len);
    clover_device dev = clover_query_user_for_device(dl);
    TEST_ASSERT_NOT_NULL(dev.name);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(clvrGetAllDevices_shouldGetAllSystemDevices);
    // requires user input:
    // RUN_TEST(clvrQueryUserForDevice_shouldReturnOneDevice);
    return UNITY_END();
}