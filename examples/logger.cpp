#define HONLY_LOGGER_TAG "Logger System"

#include "honly_logger.hpp"

int main() {
    int a = 1, b = 2;
    HONLY_LOGD("a = %d, b = %d", a, b);
    HONLY_LOGI("a = %d, b = %d", a, b);
    HONLY_LOGW("a = %d, b = %d", a, b);
    if (a != b) {
        HONLY_LOGE("a != b: a = %d, b = %d", a, b);
    }
    return 0;
}