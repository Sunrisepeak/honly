# 使用示例

```cpp
#define HONLY_LOG_TAG "HOnly"
#define HONLY_LOG_LEVEL 3 // 3 - 2 - 1 - 0

#include "honly_logmacro.hpp"

int main() {
    HONLY_LOGD("hello honly!");
    HONLY_LOGI("hello honly!");
    HONLY_LOGW("hello honly!");
    int a = 1, b = 2;
    if (a != b) {
        HONLY_LOGE("a != b: a = %d, b = %d", a, b);
    }
    return 0;
}
```